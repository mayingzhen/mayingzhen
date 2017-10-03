#include "BatchRenderable.h"


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

			i = long(a->GetMaterial()->GetShadingTechnqiue()) - long(b->GetMaterial()->GetShadingTechnqiue());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			i = int(a->GetMaterial()) - int(b->GetMaterial());
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

	void BatchRenderable::PrepareRender(RenderPassType eRPType)
	{
		std::sort(m_arrRenderList.begin(), m_arrRenderList.end(), SortDescendingLess());

		for (UINT i = 0; i < m_arrRenderList.size(); ++i)
		{
			Renderable* pRenderObj = m_arrRenderList[i];
			if (pRenderObj == NULL)
				continue;

			Technique* pTech = pRenderObj->m_pSubMaterial->GetShadingTechnqiue();
			if (eRPType == RP_ShadowDepth)
			{
				pTech = pRenderObj->m_pSubMaterial->GetShadowDepthTechnqiue();

				Uniform* pUniform = pTech->GetUniform("matLightViewProj");
				DirectonalLight* pDirLight = GetRenderSystem()->GetScene()->GetDirLight();
				ShadowMapFrustum& shadowMap = pDirLight->GetShadowMapFrustum(0);
				pTech->SetValue(pUniform, shadowMap.GetLightViewProjMatrix());
			}

			pRenderObj->PreRender(pTech);
		}
	}

	struct RenderJobData
	{
		Renderable** m_pNodeStart = NULL;
		uint32 m_nNodeCount = 0;
		UINT m_nIndex = 0;
		RenderPassType eRPType;
		RenderCommand* pCommand = NULL;
	};

	void* ParallelRender(void* rawData, void* rawData1)
	{
		RenderJobData* pJobData = reinterpret_cast<RenderJobData*>(rawData);

		pJobData->pCommand->Begin();

		Renderable** ppNodeStart = pJobData->m_pNodeStart;
		UINT32 nIndexCount = pJobData->m_nNodeCount;

		for (uint32 i = 0; i <= nIndexCount; ++i)
		{
			Renderable* pRenderable = ppNodeStart[i];

			Technique* pTech = pRenderable->m_pSubMaterial->GetShadingTechnqiue();
			if (pJobData->eRPType == RP_ShadowDepth)
			{
				pTech = pRenderable->m_pSubMaterial->GetShadowDepthTechnqiue();
			}

			pTech->CommitChanges(pJobData->pCommand);

			pJobData->pCommand->SetVertexBuffer(0, pRenderable->m_pVertexBuffer.get());

			pJobData->pCommand->SetIndexBuffer(pRenderable->m_pIndexBuffer.get());

			pJobData->pCommand->DrawRenderable(pRenderable, pTech);
		}

		pJobData->pCommand->End();

		return NULL;
	}
	
	void BatchRenderable::Render(RenderPass* pPass, RenderPassType eRPType, RenderListType eRLType)
	{
		uint32 nNumJob = GetJobScheduler()->GetNumThreads() + 1; // WorkThread + MainThread

		if (nNumJob > 1 && m_arrRenderList.size() > nNumJob)
		{
			//BEGIN_TIME(g_pTaskScheduler);

			static vector<RenderJobData> vecJobData;
			vecJobData.resize(nNumJob);

			uint32 nCountPerJob = m_arrRenderList.size() / nNumJob;

			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(nNumJob);

			for (UINT32 iJob = 0; iJob < nNumJob; ++iJob)
			{
				uint32 nStartIndex = iJob * nCountPerJob;
				uint32 nEndIndex = nStartIndex + nCountPerJob - 1;
				if (iJob == nNumJob - 1)
					nEndIndex = m_arrRenderList.size() - 1;

				ASSERT(nEndIndex >= nStartIndex);
				if (nEndIndex < nStartIndex)
					continue;

				uint32 nCount = nEndIndex - nStartIndex;

				vecJobData[iJob].m_pNodeStart = &(m_arrRenderList[nStartIndex]);

				vecJobData[iJob].m_nNodeCount = nCount;

				vecJobData[iJob].m_nIndex = iJob;

				vecJobData[iJob].eRPType = eRPType;

				vecJobData[iJob].pCommand = pPass->GetThreadCommand(iJob, eRPType, eRLType);

				void* data = &vecJobData[iJob];
				GetJobScheduler()->SubmitJob(jobGroup, ParallelRender, data, NULL, NULL);
			}

			GetJobScheduler()->WaitForGroup(jobGroup);
		}
		else
		{
			RenderCommand* pCommand = pPass->GetThreadCommand(0,eRPType,eRLType);
		
			pCommand->Begin();

			for (UINT i = 0; i < m_arrRenderList.size(); ++i)
			{
				Renderable* pRenderable = m_arrRenderList[i];
				if (pRenderable == NULL)
					continue;

                Technique* pTech = pRenderable->m_pSubMaterial->GetShadingTechnqiue();
                if (eRPType == RP_ShadowDepth)
                {
                    pTech = pRenderable->m_pSubMaterial->GetShadowDepthTechnqiue();
                }

				pCommand->SetVertexBuffer(0, pRenderable->m_pVertexBuffer.get());

				pCommand->SetIndexBuffer(pRenderable->m_pIndexBuffer.get());

				pTech->CommitChanges(pCommand);

				pCommand->DrawRenderable(pRenderable, pTech);
			}

			pCommand->End();
		}
	}

	void BatchRenderable::Clear()
	{
		m_arrRenderList.clear();
	}

}

