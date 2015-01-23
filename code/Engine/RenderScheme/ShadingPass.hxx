#include "ShadingPass.h"
#include "../RenderSystem/RenderSetting.h"
#include "HDRPostProcess.h"

namespace ma
{
	ShadingPass::ShadingPass(Scene* pScene)
		:RenderPass(pScene)
	{
		m_bIsHDRRending = false;
 	}

	void ShadingPass::Init()
	{
		m_pShadingTex = NULL;
		m_pHdrPostprocess = NULL;

		if (m_bIsHDRRending)
		{
			m_pShadingTex = GetRenderSystem()->CreateRenderTexture(-1,-1,PF_FLOAT16_RGBA);	

			RefPtr<Texture>	 pOutTagget = GetRenderSystem()->GetRenderTarget(0);

			m_pHdrPostprocess = new HDRPostProcess(m_pShadingTex.get(),pOutTagget.get());
			m_pHdrPostprocess->Init();
		}
	}

	void ShadingPass::RenderObjecList()
	{
		ColourValue cClearClor = GetRenderSystem()->GetClearColor();

		GetRenderSystem()->ClearBuffer(true,true,true,cClearClor, 1.0f, 0);

		RenderQueue* pRenderQueue = m_pScene->GetRenderQueue();

		{
			RENDER_PROFILE(RL_Solid);

			UINT nSolid = pRenderQueue->GetRenderObjNumber(RL_Solid);
			for (UINT i = 0; i < nSolid; ++i)
			{
				Renderable* pRenderObj = pRenderQueue->GetRenderObjByIndex(RL_Solid, i);
				if (pRenderObj == NULL)
					continue;

				Technique* pTech = pRenderObj->m_pMaterial->GetShadingTechnqiue();

				pRenderObj->Render(pTech);
			}
		}


		{
			RENDER_PROFILE(RL_Terrain);

			UINT nSolid = pRenderQueue->GetRenderObjNumber(RL_Terrain);
			for (UINT i = 0; i < nSolid; ++i)
			{
				Renderable* pRenderObj = pRenderQueue->GetRenderObjByIndex(RL_Terrain,i);
				if (pRenderObj == NULL)
					continue; 

				Technique* pTech = pRenderObj->m_pMaterial->GetShadingTechnqiue();

				pRenderObj->Render(pTech);
			}
		}

		{
			RENDER_PROFILE(RL_Trans);

			UINT nTrans = pRenderQueue->GetRenderObjNumber(RL_Trans);
			for (UINT i = 0; i < nTrans; ++i)
			{
				Renderable* pRenderObj = pRenderQueue->GetRenderObjByIndex(RL_Trans,i);
				if (pRenderObj == NULL)
					continue;

				Technique* pTech = pRenderObj->m_pMaterial->GetShadingTechnqiue();

				pRenderObj->Render(pTech);
			}
		}

	}

	void ShadingPass::Render()
	{
		RENDER_PROFILE(ShadingPass);

		RefPtr<Texture> pPreTarget = NULL;
		if (m_bIsHDRRending)
		{
			pPreTarget = GetRenderSystem()->SetRenderTarget(m_pShadingTex.get());
		}

		RenderObjecList();

		if (m_bIsHDRRending)
		{
			GetRenderSystem()->SetRenderTarget(pPreTarget);

			m_pHdrPostprocess->Render();
		}
	}

	void ShadingPass::ShoutDown()
	{
		m_pShadingTex = NULL;
		SAFE_DELETE(m_pHdrPostprocess);
	}
}

