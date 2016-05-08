#include "RenderScheme.h"
#include "HDRPostProcess.h"
#include "SMAAPostProcess.h"
#include "../RenderSystem/FrameBuffer.h"

namespace ma
{
	RenderScheme::RenderScheme(Scene* pScene)
	{
		m_pScene = pScene;
		m_pFrameBuffer = new FrameBuffer();
	}

	void RenderScheme::Init()
	{
		Shoutdown();

		if (m_pDeferredShadingPass)
		{
			m_pDeferredShadingPass->Init();
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
		m_pFrameBuffer->AttachDepthStencil(NULL);
		m_pFrameBuffer->AttachColor(0,NULL);
		m_pFrameBuffer->AttachColor(1,NULL);

		if (m_pDeferredShadingPass)
		{
			m_pDepthTex = GetRenderSystem()->CreateDepthStencil(-1, -1, PF_D24S8,true);
			m_pDiffuseTex = GetRenderSystem()->CreateRenderTarget(-1, -1, PF_A8R8G8B8,false,true);
			m_pNormalTex = GetRenderSystem()->CreateRenderTarget(-1, -1, PF_A8R8G8B8,false,false);

			m_pDepthSampler = CreateSamplerState(m_pDepthTex.get(),CLAMP,TFO_POINT,false);
			m_pDiffuseSampler = CreateSamplerState(m_pDiffuseTex.get(),CLAMP,TFO_POINT,true);
			m_pNormalSampler = CreateSamplerState(m_pNormalTex.get(),CLAMP,TFO_POINT,false);

			m_pFrameBuffer->AttachDepthStencil(m_pDepthTex.get());
			m_pFrameBuffer->AttachColor(0,m_pDiffuseTex.get());
			m_pFrameBuffer->AttachColor(1,m_pNormalTex.get());
		}
		else
		{
			if (m_pHDR)
			{
				m_pDiffuseTex = GetRenderSystem()->CreateRenderTarget(-1, -1, PF_FLOAT16_RGBA);
			}
			else if (m_pSMAA)
			{
				m_pDiffuseTex = GetRenderSystem()->CreateRenderTarget(-1, -1, PF_A8R8G8B8,true,true);
			}

			m_pFrameBuffer->AttachDepthStencil( GetRenderSystem()->GetDefaultDepthStencil().get() );

			if (m_pDiffuseTex)
			{
				m_pFrameBuffer->AttachColor( 0,m_pDiffuseTex.get() );
			}
			else
			{
				m_pFrameBuffer->AttachColor( 0,GetRenderSystem()->GetDefaultRenderTarget().get() );
			}
		}

		if (m_pDeferredShadingPass)
		{
			m_pDeferredShadingPass->Reset();
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

		if (m_pDeferredShadowPass)
		{
			RENDER_PROFILE(m_pDeferredShadowPass);
			m_pDeferredShadowPass->Render();
		}

		if (m_pDeferredShadingPass)
		{
			FrameBuffer fb;
			fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
			fb.AttachColor(0,GetRenderSystem()->GetDefaultRenderTarget().get());
			GetRenderSystem()->SetFrameBuffer(&fb);

			RENDER_PROFILE(m_pDeferredShadingPass);
			m_pDeferredShadingPass->Render();
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
			if (m_pSMAA == NULL)
				return;

			m_pSMAA = NULL;
		}

		Init();
		Reset();
	}

	bool RenderScheme::GetSMAAEnabled() const
	{
		return m_pSMAA != NULL;
	}

	void RenderScheme::SetDeferredShadingEnabled(bool b)
	{
		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2)
			return;

		if (b)
		{
			if (m_pDeferredShadingPass)
				return;

			m_pDeferredShadingPass = new DeferredShadingPass(m_pScene);

			GetRenderSystem()->AddShaderGlobaMacro("DEFERREDSHADING", "1");
		}
		else
		{
			if (m_pDeferredShadingPass == NULL)
				return;

			m_pDeferredShadingPass = NULL;

			GetRenderSystem()->AddShaderGlobaMacro("DEFERREDSHADING", "0");
		}

		Init();
		Reset();
	}

	bool RenderScheme::GetDeferredShadingEnabled() const
	{
		return m_pDeferredShadingPass != NULL;
	}
}
