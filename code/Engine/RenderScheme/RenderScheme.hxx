#include "RenderScheme.h"
#include "HDRPostProcess.h"
#include "SMAAPostProcess.h"
#include "AlchemyAo.h"
#include "../RenderSystem/RenderCommand.h"

namespace ma
{
	RenderScheme::RenderScheme(Scene* pScene)
	{
		m_pScene = pScene;
	}

	void RenderScheme::Init()
	{
		Shoutdown();

		if (m_pDeferredShading)
		{
			m_pDeferredShading->Init();
		}

		if (m_pDeferredShadow)
		{
			m_pDeferredShadow->Init();
		}

		if (m_pSSAO)
		{
			m_pSSAO->Init();
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
// 		m_pFrameBuffer->AttachDepthStencil(NULL);
// 		m_pFrameBuffer->AttachColor(0,NULL);
// 		m_pFrameBuffer->AttachColor(1,NULL);
		
//		m_pDiffuseTex = GetRenderSystem()->GetDefaultRenderTarget();
		if (m_pHDR)
		{
			m_pDiffuseTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1,PF_FLOAT16_RGBA,true);
		}
		else if (m_pSMAA)
		{
			m_pDiffuseTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8,true);
		}

		//m_pDepthTex = GetRenderSystem()->GetDefaultDepthStencil();
		//m_pDepthSampler = CreateSamplerState(m_pDepthTex.get(),CLAMP,TFO_POINT,false);
		//m_pFrameBuffer->AttachDepthStencil(m_pDepthTex.get());

		if (m_pDeferredShading)
		{
            m_pLinearDepthTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1,PF_FLOAT32_R,false);
            m_pLinearDepthSampler = CreateSamplerState(m_pLinearDepthTex.get(),CLAMP,TFO_POINT,false);
            //m_pTecLinearDepth = CreateTechnique("LinearDepth","screen","linearizedepth","");
            
			m_pDiffTemp = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8,true);
			m_pNormalTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8,false);

			m_pDiffTempSampler = CreateSamplerState(m_pDiffTemp.get(),CLAMP,TFO_POINT,true);
			m_pNormalSampler = CreateSamplerState(m_pNormalTex.get(),CLAMP,TFO_POINT,false);

			//m_pFrameBuffer->AttachColor(0,m_pDiffTemp.get());
			//m_pFrameBuffer->AttachColor(1,m_pNormalTex.get());
		}
		else
		{
			if (m_pDiffuseTex)
			{
				//m_pFrameBuffer->AttachColor( 0,m_pDiffuseTex.get() );
			}
			else
			{
				//m_pFrameBuffer->AttachColor( 0,GetRenderSystem()->GetDefaultRenderTarget().get() );
			}
		}

		if (m_pDeferredShading)
		{
			m_pDeferredShading->Reset();
		}

		if (m_pDeferredShadow)
		{
			m_pDeferredShadow->Reset();
		}

		if (m_pSSAO)
		{
			m_pSSAO->Reset(NULL,NULL);
		}

// 		RefPtr<Texture> pOutputTex = GetRenderSystem()->GetDefaultRenderTarget();
// 
// 		if (m_pHDR)
// 		{
// 			m_pHDR->Reset(m_pDiffuseTex.get(),pOutputTex.get());
// 		}
// 
// 		if (m_pSMAA)
// 		{
// 			m_pSMAA->Reset(m_pDiffuseTex.get(),pOutputTex.get());
// 		}
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

		RenderPass* pRenderPass = GetRenderSystem()->GetDefaultRenderPass();
		pRenderPass->Begine();

		{
			RENDER_PROFILE(RL_Mesh);
			pRenderQueue->RenderObjList(pRenderPass,RL_Mesh,RP_Shading);
		}

		{
			RENDER_PROFILE(RL_Terrain);
			pRenderQueue->RenderObjList(pRenderPass,RL_Terrain,RP_Shading);
		}

		pRenderPass->End();

// 		if (m_pLinearDepthPass)
// 		{
// 			m_pLinearDepthPass->Begine();
// 			ScreenQuad::Render(m_pTecLinearDepth.get());
// 			m_pLinearDepthPass->End();
// 		}

		if (m_pDeferredShadow)
		{
			RENDER_PROFILE(m_pDeferredShadow);
			m_pDeferredShadow->Render();
		}

		if (m_pSSAO)
		{
			m_pSSAO->Render();
		}

// 		if (m_pDeferredShadingPass)
// 		{
// 			FrameBuffer fb;
// 			fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
// 			fb.AttachColor(0,m_pDiffuseTex.get());
// 			GetRenderSystem()->SetFrameBuffer(&fb);
// 
// 			RENDER_PROFILE(m_pDeferredShadingPass);
// 			m_pDeferredShadingPass->Render();
// 		}

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
			if (m_pDeferredShading)
				return;

			//m_pDeferredShadingPass = new DeferredShadingPass(m_pScene);

			GetRenderSystem()->AddShaderGlobaMacro("DEFERREDSHADING", "1");
		}
		else
		{
			if (m_pDeferredShading == NULL)
				return;

			m_pDeferredShading = NULL;

			GetRenderSystem()->AddShaderGlobaMacro("DEFERREDSHADING", "0");
		}

		Init();
		Reset();
	}

	bool RenderScheme::GetDeferredShadingEnabled() const
	{
		return m_pDeferredShading != NULL;
	}

	void RenderScheme::SetDeferredShadowEnabled(bool b)
	{
		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2)
			return;

		if (b)
		{
			if (m_pDeferredShadow)
				return;

			//m_pDeferredShadowPass = new DeferredShadowPass(m_pScene);

			GetRenderSystem()->AddShaderGlobaMacro("USING_DEFERREDSHADOW", "1");
		}
		else
		{
			if (m_pDeferredShadow == NULL)
				return;

			m_pDeferredShadow = NULL;

			GetRenderSystem()->AddShaderGlobaMacro("USING_DEFERREDSHADOW", "0");
		}

		Init();
		Reset();
	}

	bool RenderScheme::GetDeferredShadowEnabled() const
	{
		return m_pDeferredShadow != NULL;
	}

	void RenderScheme::SetSSAOEnabled(bool b)
	{
		if (GetRenderDevice()->GetRenderDeviceType() == RenderDevice_GLES2)
			return;

		if (b)
		{
			if (m_pSSAO)
				return;

			if (!GetDeferredShadingEnabled())
				return;

			m_pSSAO = new AlchemyAo();
		}
		else
		{
			if (m_pSSAO == NULL)
				return;

			m_pSSAO = NULL;
		}

		Init();
		Reset();
	}

	bool RenderScheme::GetSSAOEnabled() const
	{
		return m_pSSAO != NULL;
	}
}

