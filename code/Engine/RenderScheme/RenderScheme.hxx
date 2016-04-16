#include "RenderScheme.h"
#include "HDRPostProcess.h"
#include "SMAAPostProcess.h"

namespace ma
{
	RenderScheme::RenderScheme(RenderScheme::Type eType,Scene* pScene)
	{
		m_eType = eType;
		m_pScene = pScene;

		if (eType == DeferredShading)
		{
			m_pDeferredLightPass = new DeferredLightPass(pScene);
		}

		//m_pSMAA = new SMAAPostProcess();
	}

	void RenderScheme::Init()
	{
		if (m_pDeferredLightPass)
		{
			m_pDeferredLightPass->Init();
		}

		if (m_pDeferredShadowPass)
		{
			m_pDeferredShadowPass->Init();
		}

		if (m_pHDR)
		{
			m_pHDR->Init();
		}

		if (m_pSMAA)
		{
			m_pSMAA->Init();
		}
	}

	void RenderScheme::Reset()
	{
		if (m_eType == DeferredShading)
		{
			if (GetDeviceCapabilities()->GetINTZSupported())
			{
				m_pDepthTex = GetRenderSystem()->CreateRenderTexture(-1, -1, PF_INTZ, USAGE_DEPTHSTENCIL);
			}
			else
			{
				m_pDepthTex = GetRenderSystem()->CreateRenderTexture(-1, -1, PF_FLOAT32_R);
			}

			m_pDiffuse = GetRenderSystem()->CreateRenderTexture(-1, -1, PF_A8R8G8B8);
			m_pNormalTex = GetRenderSystem()->CreateRenderTexture(-1, -1, PF_A8R8G8B8);
		}
		else
		{
			if (m_pHDR)
			{
				m_pDiffuse = GetRenderSystem()->CreateRenderTexture(-1, -1, PF_FLOAT16_RGBA);
			}
			else if (m_pSMAA)
			{
				m_pDiffuse = GetRenderSystem()->CreateRenderTexture(-1, -1, PF_A8R8G8B8);
			}
		}

		if (m_pDeferredLightPass)
		{
			m_pDeferredLightPass->Reset();
		}

		if (m_pDeferredShadowPass)
		{
			m_pDeferredShadowPass->Reset();
		}

		RefPtr<Texture> pOutputTex = GetRenderSystem()->GetRenderTarget(0);

		if (m_pHDR)
		{
			m_pHDR->Reset(m_pDiffuse.get(),pOutputTex.get());
		}

		if (m_pSMAA)
		{
			m_pSMAA->Reset(m_pDiffuse.get(),pOutputTex.get());
		}
	}

	void RenderScheme::Shoutdown()
	{

	}

	void RenderScheme::Render()
	{
		RenderQueue* pRenderQueue = m_pScene->GetRenderQueue();

		// DepthStencil
		if (GetDeviceCapabilities()->GetINTZSupported())
		{
			GetRenderSystem()->SetDepthStencil(m_pDepthTex);
		}

		if (m_pDiffuse)
		{
			GetRenderSystem()->SetRenderTarget(m_pDiffuse,0);
		}

		if (m_eType == DeferredShading)
		{
			GetRenderSystem()->SetRenderTarget(m_pNormalTex,1);

			if (!GetDeviceCapabilities()->GetINTZSupported())
			{
				GetRenderSystem()->SetRenderTarget(m_pDepthTex,2);
			}
		}

		ColourValue cClearClor = GetRenderSystem()->GetClearColor();

		GetRenderSystem()->ClearBuffer(true,true,true,cClearClor, 1.0f, 0);

		{
			RENDER_PROFILE(RL_Mesh);
			pRenderQueue->RenderObjList(RL_Mesh);
		}

		{
			RENDER_PROFILE(RL_Terrain);
			pRenderQueue->RenderObjList(RL_Terrain);
		}

		if (m_eType == DeferredShading)
		{
			GetRenderSystem()->SetRenderTarget(NULL,1);
			if (!GetDeviceCapabilities()->GetINTZSupported())
			{
				GetRenderSystem()->SetRenderTarget(NULL,2);
			}
		}

		if (m_pDeferredShadowPass)
		{
			RENDER_PROFILE(m_pDeferredShadowPass);
			m_pDeferredShadowPass->Render();
		}

		if (m_pDeferredLightPass)
		{
			RENDER_PROFILE(m_pDeferredLightPass);
			m_pDeferredLightPass->Render();
		}

		{
			RENDER_PROFILE(RL_Particle);
			pRenderQueue->RenderObjList(RL_Particle);
		}

		if (m_pHDR)
		{
			m_pHDR->Render();
		}

		if (m_pSMAA)
		{
			m_pSMAA->Render();
		}
	}


	RefPtr<RenderScheme> CreateRenderScheme(RenderScheme::Type eType, Scene* pScene)
	{
		RenderScheme* pRenderScheme = new RenderScheme(eType,pScene);

		pRenderScheme->Init();

		pRenderScheme->Reset();
		
 		return pRenderScheme;
	}
}
