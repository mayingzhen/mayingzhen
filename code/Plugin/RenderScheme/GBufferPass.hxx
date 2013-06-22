#include "GBufferPass.h"

namespace ma
{
	GBufferPass::GBufferPass()
	{
		m_pDepthTex = NULL;
		m_pNormalTex = NULL;

		m_pGBufferTech = NULL;
		m_pGBufferSkinTech = NULL;
	}

	void GBufferPass::Init()
	{
		m_pDepthTex = GetRenderDevice()->CreateRenderTarget();
		m_pDepthTex->Create(-1,-1,FMT_R32F);
		
		m_pNormalTex = GetRenderDevice()->CreateRenderTarget();
		m_pNormalTex->Create(-1,-1,FMT_A8R8G8B8);

		m_pGBufferTech = GetRenderDevice()->CreateShaderProgram();
		m_pGBufferTech->CreateFromShaderName("gbuffer");

		m_pGBufferSkinTech = GetRenderDevice()->CreateShaderProgram();
		m_pGBufferSkinTech->CreateFromShaderName("gbuffer","SKIN");
	}

	void GBufferPass::DoRender()
	{
		UINT uNumber = m_SolidEntry.size();
		if (uNumber <= 0)
			return;

		RenderTarget* preTarget0 = GetRenderDevice()->SetRenderTarget(m_pDepthTex,0);
		RenderTarget* preTarget1 = GetRenderDevice()->SetRenderTarget(m_pNormalTex,1);

		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0),1.0f,0.0f);


		float fNearClip = m_pMainCamera->GetNearClip();
		float fFarClip = m_pMainCamera->GetFarClip();
		Vector4 depth_near_far_invfar = Vector4(fNearClip, fFarClip, 1 / fFarClip, 0);

		for(UINT i = 0; i < uNumber; ++i)
		{
			Renderable* pRenderItem = m_SolidEntry[i];
			if (pRenderItem == NULL)
				continue;

			ShaderProgram* pCurShader = NULL;
			if (pRenderItem->m_nSkinMatrixNum != 0)
			{
				pCurShader = m_pGBufferSkinTech;
				pCurShader->Bind();
				pCurShader->SetValue("mSkinMatrixArray",pRenderItem->m_arrSkinMatrix,pRenderItem->m_nSkinMatrixNum);
			}
			else
			{
				pCurShader = m_pGBufferTech;
				pCurShader->Bind();
			}
		
 			Matrix4x4 matWVP = *(pRenderItem->m_matWorld) * m_pMainCamera->GetViewProjMatrix();
 			Matrix4x4 matWV = *(pRenderItem->m_matWorld) * m_pMainCamera->GetViewMatrix();
 			pCurShader->SetValue("worldviewprojection",matWVP);
 			pCurShader->SetValue("worldview",matWV);
 			pCurShader->SetValue("depth_near_far_invfar",depth_near_far_invfar);

			GetRenderDevice()->DrawRenderable(pRenderItem);
		}

		GetRenderDevice()->SetRenderTarget(preTarget0,0);
		GetRenderDevice()->SetRenderTarget(preTarget1,1);

	}
}