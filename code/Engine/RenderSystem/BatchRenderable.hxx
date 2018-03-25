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

		bool operator()(const Renderable* a, const Renderable* b) const
		{
			long i = long(a->GetTechnique()->GetShaderProgram()) - long(b->GetTechnique()->GetShaderProgram());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			i = long(a->m_pVertexBuffer.get()) - long(b->m_pVertexBuffer.get());
			if (i < 0)
				return true;
			else if (i > 0)
				return false;

			i = long(a->m_pIndexBuffer.get()) - long(b->m_pIndexBuffer.get());
			if (i < 0)
				return true;
			else if (i > 0)
				return false;

			i = long(a->GetTechnique()) - long(b->GetTechnique());
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

	void BatchRenderable::AddRenderObj(Renderable* pRenderObj) 
	{
		m_arrRenderList.push_back(pRenderObj);
	}

	void BatchRenderable::PrepareInstance(const std::vector<Renderable*>& batch,Technique* pTech)
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

		for (UINT i = 0; i < batch.size(); ++i)
		{
			if (!batch[i]->GetTechnique()->GetInstTech() || !batch[i]->m_bSuportInstace)
			{
				m_arrNoInsRenderList.push_back(batch[i]);
				continue;
			}

			if (pInstanceRenderable == NULL)
			{
				pInstanceRenderable = new InstanceRenderable();
			}

			pInstanceRenderable->AddRenderable(batch[i]);
		}

		if (pInstanceRenderable == NULL)
			return;

		pInstanceRenderable->Create();

		m_arrInsRenderList.push_back(pInstanceRenderable);
	}

	void BatchRenderable::PrepareInstance(RenderPassType eRPType)
	{
		if (m_arrRenderList.empty())
			return;

		m_batchTemp.clear();

		//Technique* pPreMaterial = NULL;
		ShaderProgram* pPreShader = NULL;
		VertexBuffer* pPreVB = NULL;

		for (auto iter = m_arrRenderList.begin(); iter != m_arrRenderList.end(); ++iter)
		{
			Renderable* pRenderable = *iter;

			ShaderProgram* pShader = pRenderable->GetTechnique()->GetShaderProgram();
			VertexBuffer* pVB = pRenderable->m_pVertexBuffer.get();

			if (pPreShader && pShader != pPreShader || pPreVB && pPreVB != pVB)
			{
				this->PrepareInstance(m_batchTemp, NULL);

				m_batchTemp.clear();
			}

			m_batchTemp.push_back(pRenderable);

			pPreShader = pShader;
			pPreVB = pVB;
		}

		this->PrepareInstance(m_batchTemp, NULL);

		m_batchTemp.clear();
	}

	void BatchRenderable::PrepareRender(RenderPassType eRPType)
	{
		std::sort(m_arrRenderList.begin(), m_arrRenderList.end(), SortDescendingLess());

		PrepareInstance(eRPType);

		for (UINT i = 0; i < m_arrInsRenderList.size(); ++i)
		{
			m_arrPrePareRenderList.push_back(m_arrInsRenderList[i].get());
		}

		for (UINT i = 0; i < m_arrNoInsRenderList.size(); ++i)
		{
			m_arrPrePareRenderList.push_back(m_arrNoInsRenderList[i]);
		}

// 		for (UINT i = 0; i < m_arrPrePareRenderList.size(); ++i)
// 		{
// 			Renderable* pRenderObj = m_arrPrePareRenderList[i];
// 			if (pRenderObj == NULL)
// 				continue;
// 
// 			Technique* pTech = pRenderObj->GetTechnique();
// 
// 			pRenderObj->PreRender(pTech);
// 		}
	}

	struct RenderJobData
	{
		Renderable** m_pNodeStart = NULL;
		uint32 m_nNodeCount = 0;
		UINT m_nIndex = 0;
		RenderPassType eRPType;
		RenderCommand* pCommand = NULL;
	};

	void ParallelRender(void* rawData)
	{
		RenderJobData* pJobData = reinterpret_cast<RenderJobData*>(rawData);

		pJobData->pCommand->Begin();

		Renderable** ppNodeStart = pJobData->m_pNodeStart;
		UINT32 nIndexCount = pJobData->m_nNodeCount;

		for (uint32 i = 0; i <= nIndexCount; ++i)
		{
			Renderable* pRenderable = ppNodeStart[i];

			pRenderable->PreRender(pRenderable->GetTechnique());

			pRenderable->Render(pRenderable->GetTechnique(),pJobData->pCommand);
		}

		pJobData->pCommand->End();
	}

	void BatchRenderable::Render(RenderPass* pPass, RenderPassType eRPType, RenderListType eRLType)
	{
		uint32 nNumJob = GetJobScheduler()->GetNumThreads() + 1; // WorkThread + MainThread

		if (nNumJob > 1 && m_arrPrePareRenderList.size() > nNumJob)
		{
			//BEGIN_TIME(g_pTaskScheduler);

			static vector<RenderJobData> vecJobData;
			vecJobData.resize(nNumJob);

			uint32 nCountPerJob = m_arrPrePareRenderList.size() / nNumJob;

			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(nNumJob);

			for (UINT32 iJob = 0; iJob < nNumJob; ++iJob)
			{
				uint32 nStartIndex = iJob * nCountPerJob;
				uint32 nEndIndex = nStartIndex + nCountPerJob - 1;
				if (iJob == nNumJob - 1)
					nEndIndex = m_arrPrePareRenderList.size() - 1;

				ASSERT(nEndIndex >= nStartIndex);
				if (nEndIndex < nStartIndex)
					continue;

				uint32 nCount = nEndIndex - nStartIndex;

				vecJobData[iJob].m_pNodeStart = &(m_arrPrePareRenderList[nStartIndex]);

				vecJobData[iJob].m_nNodeCount = nCount;

				vecJobData[iJob].m_nIndex = iJob;

				vecJobData[iJob].eRPType = eRPType;

				vecJobData[iJob].pCommand = pPass->GetThreadCommand(iJob, eRLType);

				void* data = &vecJobData[iJob];
				GetJobScheduler()->SubmitJob(jobGroup, 	
					[data] () {ParallelRender(data);} 
				);
			}

			GetJobScheduler()->WaitForGroup(jobGroup);
		}
		else
		{
			RenderCommand* pCommand = pPass->GetThreadCommand(0,eRLType);
		
			pCommand->Begin();

			for (UINT i = 0; i < m_arrPrePareRenderList.size(); ++i)
			{
				Renderable* pRenderable = m_arrPrePareRenderList[i];
				if (pRenderable == NULL)
					continue;

				pRenderable->PreRender(pRenderable->GetTechnique());

				pRenderable->Render(pRenderable->GetTechnique(), pCommand);
			}

			pCommand->End();
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

