#include "GBufferPass.h"

namespace ma
{
	GBufferPass::GBufferPass(Scene* pScene)
		:RenderPass(pScene)
	{
		m_pDepthTex = NULL;
		m_pNormalTex = NULL;
	}

	void GBufferPass::Init()
	{
		int nWidth = (int)m_pScene->GetViewport().width;
		int nHeight = (int)m_pScene->GetViewport().height;

		m_pNormalTex = GetRenderSystem()->CreateRenderTexture(nWidth, nHeight, PF_A8R8G8B8);
		
		if (GetDeviceCapabilities()->GetDepthTextureSupported())
			m_pDepthTex = GetRenderSystem()->CreateRenderTexture(nWidth, nHeight, PF_D24S8, USAGE_DEPTHSTENCIL);
		else
			m_pDepthTex = GetRenderSystem()->CreateRenderTexture(nWidth, nHeight, PF_FLOAT32_R);
	}

	void GBufferPass::Render()
	{
		RENDER_PROFILE(GBufferPass);

		RefPtr<Texture> pPreTarget0 = GetRenderSystem()->SetRenderTarget(m_pNormalTex,0);
		RefPtr<Texture> pPreTarget1 = NULL;

		if (GetDeviceCapabilities()->GetDepthTextureSupported())
		{
			pPreTarget1 = GetRenderSystem()->SetDepthStencil(m_pDepthTex);
		}
		else
		{
			pPreTarget1 = GetRenderSystem()->SetRenderTarget(m_pDepthTex,1);
		}

		RenderQueue* pRenderQueue = m_pScene->GetRenderQueue();

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

		UINT nSolid = pRenderQueue->GetRenderObjNumber(RL_Solid);
		for (UINT i = 0; i < nSolid; ++i)
		{
			Renderable* pSolidEntry = pRenderQueue->GetRenderObjByIndex(RL_Solid,i);
			Technique* pTech =  pSolidEntry->m_pMaterial->GetGbufferTechnqiue();
			pTech->SetParameter("shininess",Any(6.0f));
	
			GetRenderSystem()->DrawRenderable(pSolidEntry,pTech);
		}

		GetRenderSystem()->SetRenderTarget(pPreTarget0,0);
		if (GetDeviceCapabilities()->GetDepthTextureSupported())
		{
			GetRenderSystem()->SetDepthStencil(pPreTarget1);
		}
		else
		{
			GetRenderSystem()->SetRenderTarget(pPreTarget1,1);
		}
	}

	void GBufferPass::ShoutDown()
	{
		m_pDepthTex = NULL;
		m_pNormalTex = NULL;
	}

}

