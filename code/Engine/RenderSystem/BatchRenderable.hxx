#include "BatchRenderable.h"
#include "RenderSystem.h"
#include "InstanceRenderable.h"

namespace ma
{
	struct SortDescendingLess
	{
		SortDescendingLess()
		{
		}

		bool operator()(const RenderItem& a, const RenderItem& b) const
		{
			long i = long(a.m_tech->GetShaderProgram()) - long(b.m_tech->GetShaderProgram());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			i = long(a.m_renderable->m_pVertexBuffer.get()) - long(b.m_renderable->m_pVertexBuffer.get());
			if (i < 0)
				return true;
			else if (i > 0)
				return false;

			i = long(a.m_renderable->m_pIndexBuffer.get()) - long(b.m_renderable->m_pIndexBuffer.get());
			if (i < 0)
				return true;
			else if (i > 0)
				return false;

			i = long(a.m_tech) - long(b.m_tech);
			if (i<0)
				return true;
			else if(i>0)
				return false;

			return false;
		}
	};

	BatchRenderable::BatchRenderable()
	{

	}

	void BatchRenderable::AddRenderObj(Renderable* pRenderObj, Technique* pTech)
	{
		RenderItem item;
		item.m_renderable = pRenderObj;
		item.m_tech = pTech;
		m_arrRenderList.push_back(item);
	}

	void BatchRenderable::PrepareInstance(const std::vector<RenderItem>& batch)
	{
		//ASSERT(!batch.empty());
		if (batch.empty())
			return;

		if (batch.size() == 1)
		{
			m_arrNoInsRenderList.push_back(batch[0]);
			return;
		}

		RefPtr<InstanceRenderable> pInstanceRenderable;
		RefPtr<Technique> pInstanceTech;

		for (uint32_t i = 0; i < batch.size(); ++i)
		{
			Renderable* pRenderable = batch[i].m_renderable;
			Technique* pTech = batch[i].m_tech;
			if (!pTech || !pRenderable->m_bSuportInstace)
			{
				m_arrNoInsRenderList.push_back(batch[i]);
				continue;
			}

			if (pInstanceRenderable == NULL)
			{
				pInstanceRenderable = new InstanceRenderable();
				pInstanceTech = pTech->CreateInstTech();
			}

			pInstanceRenderable->AddRenderable(pRenderable);
		}

		if (pInstanceRenderable == NULL)
			return;

		pInstanceRenderable->Create();

		InstRenderItem instItem;
		instItem.m_renderable = pInstanceRenderable;
		instItem.m_tech = pInstanceTech;
		m_arrInsRenderList.push_back(instItem);
	}

	void BatchRenderable::PrepareInstance()
	{
		if (m_arrRenderList.empty())
			return;

		m_batchTemp.clear();

		ShaderProgram* pPreShader = NULL;
		VertexBuffer* pPreVB = NULL;

		for (auto iter = m_arrRenderList.begin(); iter != m_arrRenderList.end(); ++iter)
		{
			Renderable* pRenderable = iter->m_renderable;
			Technique* pTech = iter->m_tech;
			
			ShaderProgram* pShader = pTech->GetShaderProgram();
			VertexBuffer* pVB = pRenderable->m_pVertexBuffer.get();

			if ((pPreShader && pShader != pPreShader) || (pPreVB && pPreVB != pVB))
			{
				this->PrepareInstance(m_batchTemp);

				m_batchTemp.clear();
			}

			m_batchTemp.push_back(*iter);

			pPreShader = pShader;
			pPreVB = pVB;
		}

		this->PrepareInstance(m_batchTemp);

		m_batchTemp.clear();
	}

	void BatchRenderable::PrepareRender()
	{
		std::sort(m_arrRenderList.begin(), m_arrRenderList.end(), SortDescendingLess());

		PrepareInstance();

		for (uint32_t i = 0; i < m_arrInsRenderList.size(); ++i)
		{
			RenderItem item;
			item.m_renderable = m_arrInsRenderList[i].m_renderable.get();
			item.m_tech = m_arrInsRenderList[i].m_tech.get();

			m_arrPrePareRenderList.push_back(item);
		}

		for (uint32_t i = 0; i < m_arrNoInsRenderList.size(); ++i)
		{
			m_arrPrePareRenderList.push_back(m_arrNoInsRenderList[i]);
		}
	}

	void BatchRenderable::ParallelRender(RenderCommand* pCommand, RenderItem* pNodeStart, uint32_t nNodeCount)
	{
		pCommand->Begin();

		for (uint32_t i = 0; i < nNodeCount; ++i)
		{
			RenderItem& pRenderItem = pNodeStart[i];

			pRenderItem.m_renderable->PreRender(pRenderItem.m_tech);
			
			pRenderItem.m_renderable->Render(pRenderItem.m_tech, pCommand);
		}

		pCommand->End();
	}

	void BatchRenderable::Render(RenderPass* pPass, int stage)
	{
		if (m_arrPrePareRenderList.empty())
			return;

		uint32_t nNumJob = GetJobScheduler()->GetNumThreads() + 1; // WorkThread + MainThread

		if (nNumJob > 1 && m_arrPrePareRenderList.size() > nNumJob)
		{
			//BEGIN_TIME(g_pTaskScheduler);

			uint32_t nCountPerJob = m_arrPrePareRenderList.size() / nNumJob;

			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(nNumJob);

			for (uint32_t iJob = 0; iJob < nNumJob; ++iJob)
			{
				uint32_t nStartIndex = iJob * nCountPerJob;
				uint32_t nEndIndex = nStartIndex + nCountPerJob - 1;
				if (iJob == nNumJob - 1)
					nEndIndex = m_arrPrePareRenderList.size() - 1;

				ASSERT(nEndIndex >= nStartIndex);
				if (nEndIndex < nStartIndex)
					continue;

				uint32_t nCount = nEndIndex - nStartIndex + 1;

				RenderItem* ppNodeStart = &(m_arrPrePareRenderList[nStartIndex]);

				RenderCommand* pCommand = pPass->GetThreadCommand(iJob, stage);

				GetJobScheduler()->SubmitJob(jobGroup, 	[=] () {
					ParallelRender(pCommand, ppNodeStart, nCount);
				} );
			}

			GetJobScheduler()->WaitForGroup(jobGroup);
		}
		else
		{
			RenderCommand* pCommand = pPass->GetThreadCommand(0, stage);
			RenderItem* ppNodeStart = &(m_arrPrePareRenderList[0]);
			uint32_t nCount = m_arrPrePareRenderList.size();

			ParallelRender(pCommand, ppNodeStart, nCount);
		}
	}

	void BatchRenderable::Render(RenderCommand* pCommand, int stage)
	{
		for (auto& item : m_arrRenderList)
		{
			item.m_renderable->PreRender(item.m_tech);

			item.m_renderable->Render(item.m_tech, pCommand);
		}
	}

	void BatchRenderable::Compute(ComputeCommad* pCommand)
	{
		for (auto& item : m_arrRenderList)
		{
			item.m_renderable->Compute(item.m_tech, pCommand);
		}
	}

	void BatchRenderable::Clear()
	{
		m_arrRenderList.clear();

		m_arrInsRenderList.clear();
		m_arrNoInsRenderList.clear();

		m_arrPrePareRenderList.clear();
	}

}

