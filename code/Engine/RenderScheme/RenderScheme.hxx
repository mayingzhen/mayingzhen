#include "RenderScheme.h"
#include "HDRPostProcess.h"
#include "SMAAPostProcess.h"
#include "../RenderSystem/FrameBuffer.h"

namespace ma
{
	RenderScheme::RenderScheme(RenderScheme::Type eType,Scene* pScene)
	{
		m_eType = eType;
		m_pScene = pScene;
		m_pFrameBuffer = new FrameBuffer();

		if (eType == DeferredShading)
		{
			m_pDeferredLightPass = new DeferredLightPass(pScene);
		}
	}

	void RenderScheme::Init()
	{
		Shoutdown();

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
			m_pDepthTex = GetRenderSystem()->CreateDepthStencil(-1, -1, PF_D24S8,true);
			m_pDiffuseTex = GetRenderSystem()->CreateRenderTarget(-1, -1, PF_A8R8G8B8);
			m_pNormalTex = GetRenderSystem()->CreateRenderTarget(-1, -1, PF_A8R8G8B8);

			m_pFrameBuffer->AttachDepthStencil(m_pDepthTex.get());
			m_pFrameBuffer->AttachColor(0,m_pDiffuseTex.get());
			m_pFrameBuffer->AttachColor(1,m_pNormalTex.get());
		}
		else if (m_eType == Forward)
		{
			if (m_pHDR)
			{
				m_pDiffuseTex = GetRenderSystem()->CreateRenderTarget(-1, -1, PF_FLOAT16_RGBA);
			}
			else if (m_pSMAA)
			{
				m_pDiffuseTex = GetRenderSystem()->CreateRenderTarget(-1, -1, PF_A8R8G8B8);
			}

			m_pFrameBuffer->AttachDepthStencil( GetRenderSystem()->GetDefaultDepthStencil().get() );
			m_pFrameBuffer->AttachColor( 0,GetRenderSystem()->GetDefaultRenderTarget().get() );
		}

		if (m_pDeferredLightPass)
		{
			m_pDeferredLightPass->Reset();
		}

		if (m_pDeferredShadowPass)
		{
			m_pDeferredShadowPass->Reset();
		}

		RefPtr<Texture> pOutputTex = GetRenderSystem()->GetDefaultRenderTarget();

		if (m_pHDR)
		{
			m_pHDR->Reset(m_pDiffuseTex.get(),pOutputTex.get());
		}

		if (m_pSMAA)
		{
			m_pSMAA->Reset(m_pDiffuseTex.get(),pOutputTex.get());
		}
	}

	void RenderScheme::Shoutdown()
	{
		m_pDepthTex = NULL;
		m_pNormalTex = NULL;
		m_pDiffuseTex = NULL;
	}

	void RenderScheme::Render()
	{
		RenderQueue* pRenderQueue = m_pScene->GetRenderQueue();

		GetRenderSystem()->SetFrameBuffer(m_pFrameBuffer.get());

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

// 		if (m_eType == DeferredShading)
// 		{
// 			GetRenderSystem()->SetRenderTarget(NULL,1);
// 			GetRenderSystem()->SetRenderTarget(NULL,2);
// 		}

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

	void RenderScheme::SetSMAAEnabled(bool b)
	{
		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2)
			return;

		if (b)
		{
			if (m_pSMAA != NULL)
				return;

			m_pSMAA = new SMAAPostProcess();
		}
		else
		{
			m_pSMAA = NULL;
		}

		Init();
		Reset();
	}

	bool RenderScheme::GetSMAAEnabled() const
	{
		return m_pSMAA != NULL;
	}


	RefPtr<RenderScheme> CreateRenderScheme(RenderScheme::Type eType, Scene* pScene)
	{
		RenderScheme* pRenderScheme = new RenderScheme(eType,pScene);

		pRenderScheme->Init();

		pRenderScheme->Reset();
		
 		return pRenderScheme;
	}
}
