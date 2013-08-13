#include "Shadow.h"

namespace ma
{
	void Shadow::Init()
	{
		m_pShadowTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_R16F);

		for (UINT i = 0; i < Camera::NUM_PSSM; ++i)
		{
			m_arrSMF[i] = new ShadowMapFrustum();
			m_arrSMF[i]->Init();
		}
	}

	void Shadow::DoRender()
	{

	}

	void Shadow::Update()
	{
		for (UINT i = 0; i < m_arrSMF.size(); ++i)
		{
			m_arrSMF[i]->Update();
		}
	}

	void Shadow::ShadowDepthPass()
	{
		for (UINT i = 0; i < m_arrSMF.size(); ++i)
		{
			m_arrSMF[i]->ShadowDepthPass();
		}
	}

	void Shadow::DeferredShadow()
	{
// 		RenderTarget* pPreRT = GetRenderDevice()->SetRenderTarget(m_pShadowTex);
// 
// 		GetRenderDevice()->ClearBuffer(true,true,true,Color(1,1,1,0),1,0);
// 
// 		Matrix4x4 invView;
// 		MatrixInverse(&invView, NULL, &m_pCamera->GetViewMatrix());
// 
// 		Matrix4x4 viwToLightProjArray[Camera::NUM_PSSM];
// 		Matrix4x4 wordToLightView[Camera::NUM_PSSM];
// 
// 		for (UINT i = 0; i < Camera::NUM_PSSM; ++i)
// 		{
// 			ShadowMapFrustum* pSMF = m_arrSMF[i];
// 			if (m_arrSMF == NULL)
// 				continue;
// 
// 			viwToLightProjArray[i] = invView * pSMF->GetViewProjMarix() * pSMF->GetTexScaleBiasMatrix();
// 
// 			char strTexShadowMap[MAX_PATH] = {0};
// 			_snprintf(strTexShadowMap, sizeof(strTexShadowMap), "TexShadowMap%d", i);
// 			//hr = pCurEffect->SetTexture( strTexShadowMap, pShadowMap->GetDepthTexture() ) ;
// 		}
// 
// 		m_pScreenQuad->Render(m_pMaterDeferred);
// 
// 
// 		GetRenderDevice()->SetRenderTarget(pPreRT);
	}

	void Shadow::ShadowBlur()
	{

	}
	
}