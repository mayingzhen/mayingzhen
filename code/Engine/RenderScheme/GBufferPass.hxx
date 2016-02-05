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
		
		if (GetDeviceCapabilities()->GetINTZSupported())
			m_pDepthTex = GetRenderSystem()->CreateRenderTexture(nWidth, nHeight, PF_INTZ, USAGE_DEPTHSTENCIL);
		else
			m_pDepthTex = GetRenderSystem()->CreateRenderTexture(nWidth, nHeight, PF_FLOAT32_R);

		m_pDiffuse = GetRenderSystem()->CreateRenderTexture(nWidth, nHeight, PF_A8R8G8B8);
	}

	void GBufferPass::Render()
	{
		RENDER_PROFILE(GBufferPass);
		
		RefPtr<Texture> pPreTarget0 = GetRenderSystem()->SetRenderTarget(m_pDiffuse,0);
		RefPtr<Texture> pPreTarget1 = GetRenderSystem()->SetRenderTarget(m_pNormalTex,1);
		RefPtr<Texture> pPreTarget2 = NULL;

		if (GetDeviceCapabilities()->GetINTZSupported())
		{
			pPreTarget2 = GetRenderSystem()->SetDepthStencil(m_pDepthTex);
		}
		else
		{
			pPreTarget2 = GetRenderSystem()->SetRenderTarget(m_pDepthTex,2);
		}

		RenderQueue* pRenderQueue = m_pScene->GetRenderQueue();

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::Black, 1.0f, 0);

		{
			RENDER_PROFILE(RL_Solid);

			UINT nSolid = pRenderQueue->GetRenderObjNumber(RL_Solid);
			for (UINT i = 0; i < nSolid; ++i)
			{
				Renderable* pRenderObj = pRenderQueue->GetRenderObjByIndex(RL_Solid, i);
				if (pRenderObj == NULL)
					continue;

				Technique* pTech = pRenderObj->m_pSubMaterial->GetShadingTechnqiue();

				pRenderObj->Render(pTech);
			}
		}

		{
			RENDER_PROFILE(RL_TerrainBody);

			UINT nSolid = pRenderQueue->GetRenderObjNumber(RL_TerrainBody);
			for (UINT i = 0; i < nSolid; ++i)
			{
				Renderable* pRenderObj = pRenderQueue->GetRenderObjByIndex(RL_TerrainBody,i);
				if (pRenderObj == NULL)
					continue; 

				Technique* pTech = pRenderObj->m_pSubMaterial->GetShadingTechnqiue();

				pRenderObj->Render(pTech);
			}
		}

		{
			RENDER_PROFILE(RL_TerrainBorder);

			UINT nSolid = pRenderQueue->GetRenderObjNumber(RL_TerrainBorder);
			for (UINT i = 0; i < nSolid; ++i)
			{
				Renderable* pRenderObj = pRenderQueue->GetRenderObjByIndex(RL_TerrainBorder,i);
				if (pRenderObj == NULL)
					continue; 

				Technique* pTech = pRenderObj->m_pSubMaterial->GetShadingTechnqiue();

				pRenderObj->Render(pTech);
			}
		}

		GetRenderSystem()->SetRenderTarget(pPreTarget0,0);
		GetRenderSystem()->SetRenderTarget(pPreTarget1,1);
		if (GetDeviceCapabilities()->GetINTZSupported())
		{
			GetRenderSystem()->SetDepthStencil(pPreTarget2);
		}
		else
		{
			GetRenderSystem()->SetRenderTarget(pPreTarget2,2);
		}
	}

	void GBufferPass::ShoutDown()
	{
		m_pDepthTex = NULL;
		m_pNormalTex = NULL;
		m_pDiffuse = NULL;
	}

}

