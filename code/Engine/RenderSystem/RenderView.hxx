#include "RenderView.h"

namespace ma
{
	MainRenderView::MainRenderView()
	{
		m_pRenderStep = new MainRenderStep();
	}

	MainRenderView::~MainRenderView()
	{

	}

	void MainRenderView::Update()
	{

	}

	void MainRenderView::Render()
	{
		MICROPROFILE_SCOPEI("", "MainRenderView::Render", 0);

		m_arrRenderProxy.clear();

		//typedef std::vector<RenderProxy*> VEC_OBJ;
		//static VEC_OBJ vecObj;
		m_pScene->GetCullTree()->FindObjectsIn(&m_pCamera->GetFrustum(), -1, m_arrRenderProxy);

		RenderQueue* pGbufferQueue = m_pRenderStep->m_pGbufferStep->m_pRenderQueue.get();
		RenderQueue* pTransluceQueue = m_pRenderStep->m_pTransluceStep->m_pRenderQueue.get();

		pGbufferQueue->Clear();
		pGbufferQueue->SetCamera(m_pCamera.get());
		pGbufferQueue->SetMainLight(m_pScene->GetMainDirLight(), m_pScene->GetAmbientColor());

		pTransluceQueue->Clear();
		pTransluceQueue->SetCamera(m_pCamera.get());
		pTransluceQueue->SetMainLight(m_pScene->GetMainDirLight(), m_pScene->GetAmbientColor());

		for (uint32_t i = 0; i < m_arrRenderProxy.size(); ++i)
		{
			//m_arrRenderComp[i]->GetSceneNode()->SetLastVisibleFrame(GetTimer()->GetFrameCount());

			if (m_arrRenderProxy[i]->GetTransluce())
			{
				m_arrRenderProxy[i]->Render(pRenderQueue, m_pRenderStep->GetTranslucePass());
			}
			else
			{
				m_arrRenderProxy[i]->Render(pRenderQueue, m_pRenderStep->GetGpufferPass());
			}
		}

		m_pRenderStep->Render();
	}


}