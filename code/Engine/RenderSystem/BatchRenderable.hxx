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
			long i = long(a->GetMaterial()->GetShadingTechnqiue()->GetShaderProgram()) - long(b->GetMaterial()->GetShadingTechnqiue()->GetShaderProgram());
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

			i = long(a->GetMaterial()->GetShadingTechnqiue()) - long(b->GetMaterial()->GetShadingTechnqiue());
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

	void BatchRenderable::PrepareInstance(const std::vector<Renderable*>& batch, RenderPassType eRPType)
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
			SubMaterial* pMaterial = batch[i]->GetMaterial();
			Technique* pTech = pMaterial->GetShadingInstTechnqiue();
			if (eRPType == RP_ShadowDepth)
			{
				pTech = pMaterial->GetShadowDepthInstTechnqiue();
			}
			if (!pTech || !batch[i]->m_bSuportInstace)
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
			
			SubMaterial* pMaterial = pRenderable->GetMaterial();
			Technique* pTech = pMaterial->GetShadingTechnqiue();
			if (eRPType == RP_ShadowDepth)
			{
				pTech = pMaterial->GetShadowDepthTechnqiue();
			}
			ShaderProgram* pShader = pTech->GetShaderProgram();
			VertexBuffer* pVB = pRenderable->m_pVertexBuffer.get();

			if (pPreShader && pShader != pPreShader || pPreVB && pPreVB != pVB)
			{
				this->PrepareInstance(m_batchTemp, eRPType);

				m_batchTemp.clear();
			}

			m_batchTemp.push_back(pRenderable);

			pPreShader = pShader;
			pPreVB = pVB;
		}

		this->PrepareInstance(m_batchTemp, eRPType);

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

	Technique* GetTechnique(Renderable* pRenderable, RenderPassType eRPType, bool bInstance)
	{
		SubMaterial* pMaterial = pRenderable->GetMaterial();

		Technique* pTech = NULL;

		if (eRPType == RP_Shading)
		{
			pTech = pMaterial->GetShadingTechnqiue();
			if (bInstance)
			{
				pTech = pMaterial->GetShadingInstTechnqiue();
			}
		}
		else if (eRPType == RP_ShadowDepth)
		{
			pTech = pMaterial->GetShadowDepthTechnqiue();
			if (bInstance)
			{
				pTech = pMaterial->GetShadowDepthInstTechnqiue();
			}
		}

		return pTech;
	}

	void ParallelRender(RenderCommand* pCommand, Renderable** pNodeStart, uint32 nNodeCount,
		RenderPassType eRPType, UINT nStartIndex, UINT nIntanceListCount)
	{
		pCommand->Begin();

		for (uint32 i = 0; i < nNodeCount; ++i)
		{
			Renderable* pRenderable = pNodeStart[i];

			bool bInstance = nStartIndex + i < nIntanceListCount;

			Technique* pTech = GetTechnique(pRenderable, eRPType, bInstance);
			pRenderable->PreRender(pTech);

			pRenderable->Render(pTech, pCommand);
		}

		pCommand->End();
	}

	void BatchRenderable::Render(RenderPass* pPass, RenderPassType eRPType, RenderListType eRLType)
	{
		if (m_arrPrePareRenderList.empty())
			return;

		uint32 nNumJob = GetJobScheduler()->GetNumThreads() + 1; // WorkThread + MainThread

		if (nNumJob > 1 && m_arrPrePareRenderList.size() > nNumJob)
		{
			//BEGIN_TIME(g_pTaskScheduler);

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

				uint32 nCount = nEndIndex - nStartIndex + 1;

				Renderable** ppNodeStart = &(m_arrPrePareRenderList[nStartIndex]);

				RenderCommand* pCommand = pPass->GetThreadCommand(iJob, eRLType);

				GetJobScheduler()->SubmitJob(jobGroup, 	[=] () {
					ParallelRender(pCommand, ppNodeStart, nCount, eRPType, nStartIndex, m_arrInsRenderList.size());
				} );
			}

			GetJobScheduler()->WaitForGroup(jobGroup);
		}
		else
		{
			RenderCommand* pCommand = pPass->GetThreadCommand(0,eRLType);
			Renderable** ppNodeStart = &(m_arrPrePareRenderList[0]);
			uint32 nCount = m_arrPrePareRenderList.size();

			ParallelRender(pCommand, ppNodeStart, nCount, eRPType, 0, m_arrInsRenderList.size());
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

