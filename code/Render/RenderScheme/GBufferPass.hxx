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

		m_pDepthTex = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_R32F);
		m_pNormalTex = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
	}

	void GBufferPass::Render()
	{
		RENDER_PROFILE(GBufferPass);

		Texture* pPreTarget0 = GetRenderSystem()->SetRenderTarget(m_pDepthTex,0);
		Texture* pPreTarget1 = GetRenderSystem()->SetRenderTarget(m_pNormalTex,1);

		RenderQueue* pRenderQueue = GetRenderSystem()->GetRenderQueue();

		GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

		UINT nSolid = pRenderQueue->GetRenderObjNumber(RL_Solid);
		for (UINT i = 0; i < nSolid; ++i)
		{
			Renderable* pSolidEntry = pRenderQueue->GetRenderObjByIndex(RL_Solid,i);
			Technique* pTech =  pSolidEntry->m_pMaterial->GetTechnqiue("Gbuffer");
			pTech->GetParameter("shininess")->setVector4(Vector4(6.0f,0,0,0)); 
	
			GetRenderSystem()->DrawRenderable(pSolidEntry,pTech);
		}

		GetRenderSystem()->SetRenderTarget(pPreTarget0,0);
		GetRenderSystem()->SetRenderTarget(pPreTarget1,1);
	}

	void GBufferPass::ShoutDown()
	{

	}

}

