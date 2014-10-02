#include "RenderView.h"
#include "FrustumCullQuery.h"

namespace ma
{
	RenderView::RenderView(Scene* pScene,Camera* pCamea,RenderScheme* pRS,Texture* pRT,Rectangle rc)
	{
		m_pScene = pScene ? pScene : new Scene("default");
		m_pRenderScheme = pRS ? pRS : new RenderScheme();
		m_pRenderTarget = pRT;
		m_viewport = rc;

		if (pCamea == NULL)
		{
			SceneNode* pCamera = m_pScene->CreateNode("defaultCamera");
			m_pCamera = pCamera->CreateComponent<Camera>();
		}
	}


	void RenderView::Update()
	{
		profile_code();

		ASSERT(m_pCamera && m_pScene && m_pScene->GetCullTree());
		if (m_pCamera == NULL || m_pScene == NULL || m_pScene->GetCullTree() == NULL)
			return;
	
		m_arrRenderComp.clear();
		FrustumCullQuery frustumQuery(m_pCamera->GetFrustum(),m_arrRenderComp);
		m_pScene->GetCullTree()->FindObjectsIn(frustumQuery);

		for (UINT i = 0; i < m_arrRenderComp.size(); ++i)
		{
			RenderComponent* pGameObj = m_arrRenderComp[i];

			pGameObj->Show(m_pCamera.get());
		}
	}

	void RenderView::Render()
	{
		if (m_pRenderTarget)
		{
			GetRenderSystem()->SetRenderTarget(m_pRenderTarget.get());
		}

		GetRenderSystem()->SetViewPort(m_viewport);
	
		GetRenderContext()->SetCamera(m_pCamera.get());

		if (m_pRenderScheme)
			m_pRenderScheme->Render();

		GetLineRender()->Render();

		//if (m_bRenderUI)
		//	GetUISystem()->Render();
	}
}
