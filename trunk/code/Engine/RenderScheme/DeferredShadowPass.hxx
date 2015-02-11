#include "DeferredShadowPass.h"

namespace ma
{
	DeferredShadowPass::DeferredShadowPass(Scene* pScene)
		:RenderPass(pScene)
	{

	}

	void DeferredShadowPass::Init()
	{
		int nWidth = (int)m_pScene->GetViewport().width;
		int nHeight = (int)m_pScene->GetViewport().height;

		m_pShadowTex = GetRenderSystem()->CreateRenderTexture(nWidth, nHeight, PF_FLOAT16_R);

		m_pDefferedShadow = CreateTechnique("DeferredShadow","DefferedShadow","DefferedShadow","");

		m_pDefferedShadow->GetRenderState().m_bDepthWrite = false;
	}

	void DeferredShadowPass::Render()
	{
		RENDER_PROFILE(DeferredShadowPass);

		RenderShadowCSM* pSun = m_pScene->GetSunShaow();
		if (!pSun->GetEnabled())
			return;

		RefPtr<Texture> pPreTarget = GetRenderSystem()->SetRenderTarget(m_pShadowTex);

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::Black,1,0);

		ScreenQuad::Render(m_pDefferedShadow.get());

		GetRenderSystem()->SetRenderTarget(pPreTarget);
	}

	void DeferredShadowPass::ShoutDown()
	{

	}

}