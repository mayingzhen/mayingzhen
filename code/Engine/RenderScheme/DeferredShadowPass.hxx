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

		m_pFrustumMaterial = CreateTechnique("frustumclipvolume","frustumclipvolume","frustumclipvolume","");
		m_pDefferedShadow = CreateTechnique("DeferredShadow","DefferedShadow","DefferedShadow","");

		m_pDefferedShadow->GetRenderState().SetDepthWrite(false);
	}

	void DeferredShadowPass::Render()
	{
		RENDER_PROFILE(DeferredShadowPass);

// 		for (uint32 i = 0; i < m_pScene->GetVisibleLightNum(); ++i)
// 		{
// 
// 		}
// 
// 		RefPtr<Texture> pPreTarget = GetRenderSystem()->SetRenderTarget(m_pShadowTex);
// 
// 		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::Black,1,0);
// 		
// 		m_pFrustumMaterial->Bind();
// 
// 		GetRenderSystem()->SetStencilCheckEnabled(true);
// 		for (int i = pRenderShadow->GetCurSplitCount() - 1; i >= 0; --i) // ´ÓºóÍùÇ°
// 		{
// 			ShadowMapFrustum& shadowMapFru = pRenderShadow->GetShadowMapFrustum(i);
// 			if ( !shadowMapFru.GetDraw() )
// 				continue;
// 
// 			GetRenderSystem()->SetStencilBufferParams(CMPF_ALWAYS_PASS, i + 1, 0xFFFFffff, 0xFFFFffff,
// 				SOP_KEEP, SOP_REPLACE, SOP_KEEP, false);
// 
// 			//CGpuProgram* pFrustumShader = m_pFrustumMaterial->GetGpuProgram();
// 			//pFrustumShader->BeginActivate(pCamera);
// 			
// 			ShaderProgram* pShader = m_pFrustumMaterial->GetShaderProgram();
// 			
// 			Matrix4 matFrum = pRenderShadow->GetShadowMapFrustum(i).GetLightViewProjMatrix().inverse();
// 			GetRenderSystem()->SetValue( pShader->GetUniform("matFrustum"), matFrum );
// 
// 			//m_pFrustumMaterial->Activate(true);
// 
// 			//GetRenderSystem()->SetVertexBuffer(0, m_pFrustumVertexData.get());
// 			//GetRenderSystem()->SetIndexBuffer(m_pFrustumIndexData->pIndexBuffer.get());
// 
// 			//GetRenderSystem()->Draw(PT_TRIANGLESTRIP, m_pFrustumVertexData.get(), m_pFrustumIndexData.get(), 1);
// 		}
// 
// 		m_pFrustumMaterial->UnBind();
// 		
// // 		ShaderProgram* pShader = m_pDefferedShadow->GetShaderProgram();
// // 
// // 	 	GetRenderSystem()->SetValue(pShader->GetUniform("vStoWBasisX"),pSun->m_vWBasisX[0]);
// // 		GetRenderSystem()->SetValue(pShader->GetUniform("vStoWBasisY"),pSun->m_vWBasisY[0]);
// // 		GetRenderSystem()->SetValue(pShader->GetUniform("vStoWBasisZ"),pSun->m_vWBasisZ[0]);
// // 		GetRenderSystem()->SetValue(pShader->GetUniform("vCamPos"),pSun->m_vShadowCamPos[0]);
// // 
// // 		ScreenQuad::Render(m_pDefferedShadow.get());
// 
// 		GetRenderSystem()->SetRenderTarget(pPreTarget);
	}

	void DeferredShadowPass::ShoutDown()
	{

	}

}