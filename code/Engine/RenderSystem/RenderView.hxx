#include "RenderView.h"

namespace ma
{
	void MainRenderView::Update()
	{

	}

	void MainRenderView::Render()
	{
		MICROPROFILE_SCOPEI("", "MainRenderView::Render", 0);

		RenderStep* render_step = GetRenderSystem()->GetBaseRender();

		RenderQueue* pRenderQueue = render_step->m_pRenderQueue[GetRenderSystem()->CurThreadFill()].get();

		m_arrRenderComp.clear();

		typedef vector<RenderComponent*> VEC_OBJ;
		static VEC_OBJ vecObj;
		m_pScene->GetCullTree()->FindObjectsIn(&m_pCamera->GetFrustum(), -1, vecObj);

		uint32_t nNodeCount = vecObj.size();
		m_arrRenderComp.resize(nNodeCount);
		for (uint32_t mm = 0; mm < nNodeCount; ++mm)
		{
			m_arrRenderComp[mm] = vecObj[mm];
		}
		vecObj.clear();

		pRenderQueue->Clear();

		pRenderQueue->SetCamera(m_pCamera.get());
		pRenderQueue->SetMainLight(m_pMainDirLight.get(), m_cAmbientColor);

		for (uint32_t i = 0; i < m_arrRenderComp.size(); ++i)
		{
			m_arrRenderComp[i]->GetSceneNode()->SetLastVisibleFrame(GetTimer()->GetFrameCount());

			m_arrRenderComp[i]->Render(pRenderQueue);
		}

// 		if (GetJobScheduler()->GetNumThreads() > 0)
// 		{
// 			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(m_vecParallelShow.size());
// 			for (uint32_t i = 0; i < m_vecParallelShow.size(); ++i)
// 			{
// 				Component* pComp = m_vecParallelUpdate[i].get();
// 				Camera* pCamera = m_pCamera.get();
// 
// 				GetJobScheduler()->SubmitJob(jobGroup,
// 					[pComp, pCamera]() { pComp->ParallelShow(pCamera); }
// 				);
// 			}
// 			GetJobScheduler()->WaitForGroup(jobGroup);
// 			m_vecParallelShow.clear();
// 		}
// 		else
// 		{
// 			for (uint32_t i = 0; i < m_vecParallelShow.size(); ++i)
// 			{
// 				Component* pComp = m_vecParallelUpdate[i].get();
// 				Camera* pCamera = m_pCamera.get();
// 				pComp->ParallelShow(pCamera);
// 			}
// 		}


//		GetRenderSystem()->AddRenderStep(render_step);

		m_pRenderStep->Render();
	}

	class ShadowMapRenderView : public RenderView
	{

	};

}