#include "Shadow.h"

namespace ma
{
	Shadow*	gpShadow = NULL;

	Shadow*	GetShadowSystem()
	{
		return gpShadow;
	}

	void Shadow::Init()
	{
		gpShadow = this;

		m_fShadowMapSize = 2048.0f;

		m_pShadowTex = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_R16F);

		m_bHWShadowMap = GetRenderDevice()->CheckTextureFormat(FMT_D24S8,USAGE_DEPTHSTENCIL);


		m_pDefferedShadow = new Technique("DeferredShadow","DefferedShadow","SCEERN_LIGHT");

		m_pDefferedShadow->GetRenderState().m_bDepthWrite = false;
	}


	void Shadow::ShadowDepthPass()
	{
		RENDER_PROFILE(ShadowDepthPass);

		UINT nLight = GetRenderSystem()->GetLightNumber();
		for (UINT iLight = 0; iLight < nLight; ++iLight)
		{
			Light* pLight = GetRenderSystem()->GetLightByIndex(iLight);
			if ( !pLight->IsCreateShadow() )
				continue;
			
			int nSMF = pLight->GetShadowFrustumNumber();
			for (UINT iSMF = 0; iSMF < nSMF; ++iSMF)
			{
				ShadowMapFrustum* pSMF = pLight->GetShadowFrustumByIndex(iSMF);
				pSMF->ShadowDepthPass();
			}
		}
	}

	void Shadow::DeferredShadow()
	{
		RENDER_PROFILE(DeferredShadow);

		GetRenderSystem()->PushRenderTarget(m_pShadowTex);

		GetRenderSystem()->ClearBuffer(true,true,true,Color(1,1,1,0),1,0);

		Matrix4x4 invView;
		MatrixInverse(&invView, NULL, &GetRenderSystem()->GetViewMatrix());

		UINT nLight = GetRenderSystem()->GetLightNumber();
		for (UINT iLight = 0; iLight < nLight; ++iLight)
		{
			Light* pLight = GetRenderSystem()->GetLightByIndex(iLight);
			if (pLight->GetLightType() == LIGHT_DIRECTIONAL)
			{
				DirectonalLight* pDirLight = (DirectonalLight*)pLight;

				Matrix4x4 viwToLightProjArray[DirectonalLight::NUM_PSSM];
				//Matrix4x4 wordToLightView[DirectonalLight::NUM_PSSM];

				int nSMF = pLight->GetShadowFrustumNumber();
				for (UINT iSMF = 0; iSMF < nSMF; ++iSMF)
				{
					ShadowMapFrustum* pSMF = pLight->GetShadowFrustumByIndex(iSMF);

					viwToLightProjArray[iSMF] = invView * pSMF->GetViewMarix() * pSMF->GetProjMatrix() * pSMF->GetTexScaleBiasMat();
					//wordToLightView[iSMF] = invView * pSMF->GetViewMarix();

					char pszTexShadowMap[MAX_PATH] = {0};
					_snprintf(pszTexShadowMap, sizeof(pszTexShadowMap), "SamplerShadowMap%d", iSMF);
					m_pDefferedShadow->GetParameter(pszTexShadowMap)->setTexture( pSMF->GetDepthTexture() );
				}

				float fShadowMapSize = GetShadowMapSize();
				Vector4 uShadowMapTexelSize(fShadowMapSize, 1.0f / fShadowMapSize, 0, 0);
				m_pDefferedShadow->GetParameter("u_shadowMapTexelSize")->setFloatArray(uShadowMapTexelSize,4);

				m_pDefferedShadow->GetParameter("viwToLightProjArray")->setMatrixArray(viwToLightProjArray,nSMF);
				//m_pEffect->GetParameter("wordLightView")->setMatrixArray(wordToLightView,nSMF);
				m_pDefferedShadow->GetParameter("splitPos")->setFloatArray(pDirLight->GetSplitPos(),nSMF);

				ScreenQuad::Render(m_pDefferedShadow);

				break;
			}

		}

		GetRenderSystem()->PopRenderTargert();
	}

	void Shadow::ShadowBlur()
	{

	}
	
}