#include "GBufferPass.h"

namespace ma
{
	static GBufferPass* gpGBufferPass = NULL;

	GBufferPass* GetBuffferPass() {return gpGBufferPass;}

	IMPL_OBJECT(GBufferPass,RenderPass);

	GBufferPass::GBufferPass()
	{
		m_pDepthTex = NULL;
		m_pNormalTex = NULL;
	}

	void GBufferPass::Init()
	{
		gpGBufferPass = this;

		GetRenderSetting()->m_bDefferLight = true;

		m_pNormalTex = GetRenderSystem()->CreateRenderTarget(-1,-1,PF_A8R8G8B8);
		
		if (GetDeviceCapabilities()->GetDepthTextureSupported())
			m_pDepthTex = GetRenderSystem()->CreateRenderTarget(-1,-1,PF_D24S8,true);
		else
			m_pDepthTex = GetRenderSystem()->CreateRenderTarget(-1,-1,PF_FLOAT32_R);
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

		RenderQueue* pRenderQueue = GetRenderSystem()->GetRenderQueue();

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

		UINT nSolid = pRenderQueue->GetRenderObjNumber(RL_Solid);
		for (UINT i = 0; i < nSolid; ++i)
		{
			Renderable* pSolidEntry = pRenderQueue->GetRenderObjByIndex(RL_Solid,i);
			Technique* pTech =  pSolidEntry->m_pMaterial->GetTechnqiueByName("Gbuffer");
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

