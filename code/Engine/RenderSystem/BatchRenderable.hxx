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

	struct RenderTask
	{
		MT_DECLARE_TASK(RenderTask, MT::StackRequirements::STANDARD, MT::TaskPriority::NORMAL, MT::Color::Blue);

		Renderable** m_pNodeStart;
		uint32 m_nNodeCount;
		UINT m_nIndex;
		RenderPassType eRPType;
		RenderCommand* pCommand;

		void Do(MT::FiberContext&)
		{
			pCommand->Begin();

			Renderable** ppNodeStart = m_pNodeStart;
			UINT32 nIndexCount = m_nNodeCount;

			for (uint32 i = 0; i <= nIndexCount; ++i)
			{
				Renderable* pRenderable = ppNodeStart[i];

				Technique* pTech = pRenderable->m_pSubMaterial->GetShadingTechnqiue();
				if (eRPType == RP_ShadowDepth)
				{
					pTech = pRenderable->m_pSubMaterial->GetShadowDepthTechnqiue();
				}

				pTech->CommitChanges(pCommand);

				pCommand->SetVertexBuffer(0, pRenderable->m_pVertexBuffer.get());

				pCommand->SetIndexBuffer(pRenderable->m_pIndexBuffer.get());

				pCommand->DrawRenderable(pRenderable, pTech);
			}

			pCommand->End();
		}
	};

	void BatchRenderable::Render(RenderPass* pPass, RenderPassType eRPType, RenderListType eRLType)
	{
		uint32 nNumJob = MT::g_pTaskScheduler ? MT::g_pTaskScheduler->GetWorkersCount() : 0;

		if (/*MT::g_pTaskScheduler && m_arrRenderList.size() > nNumJob*/0)
		{
			//BEGIN_TIME(g_pTaskScheduler);

			static std::vector<RenderTask> vecJobData;
			vecJobData.resize(nNumJob);

			uint32 nCountPerJob = m_arrRenderList.size() / nNumJob;

			UINT32 iJob = 0;
			for (; iJob < nNumJob; ++iJob)
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
			}

			MT::g_pTaskScheduler->RunAsync(MT::TaskGroup::Default(), &vecJobData[0], vecJobData.size());
			MT::g_pTaskScheduler->WaitGroup(MT::TaskGroup::Default(), -1);
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

