#include "DeferredShadowPass.h"

namespace ma
{
	static DeferredShadowPass* gpDeferredShadowPass = NULL;

	DeferredShadowPass*	GetDeferredShadowPass() { return gpDeferredShadowPass; }

	IMPL_OBJECT(DeferredShadowPass,RenderPass);

	void DeferredShadowPass::Init()
	{
		gpDeferredShadowPass = this;

		m_pShadowTex = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_R16F);

		//m_bHWShadowMap = GetRenderDevice()->CheckTextureFormat(FMT_D24S8,USAGE_DEPTHSTENCIL);

		std::string strShaderDefine = "SCEERN_LIGHT";
		if ( GetRenderSetting()->m_bHWShadowMap )
		{
			strShaderDefine += ";HWPCF";
		}

		m_pDefferedShadow = new Technique("DeferredShadow","DefferedShadow","DefferedShadow",strShaderDefine.c_str());

		m_pDefferedShadow->GetRenderState().m_bDepthWrite = false;
	}

	void DeferredShadowPass::Render()
	{
		RENDER_PROFILE(DeferredShadowPass);

		Texture* pPreTarget = GetRenderSystem()->SetRenderTarget(m_pShadowTex);

		GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0),1,0);

		Matrix4x4 invView;
		MatrixInverse(&invView, NULL, &GetRenderContext()->GetViewMatrix());

		UINT nLight = GetLightSystem()->GetLightNumber();
		for (UINT iLight = 0; iLight < nLight; ++iLight)
		{
			Light* pLight = GetLightSystem()->GetLightByIndex(iLight);
			if (pLight->GetLightType() == LIGHT_DIRECTIONAL)
			{
				DirectonalLight* pDirLight = (DirectonalLight*)pLight;

				Matrix4x4 viwToLightProjArray[DirectonalLight::NUM_PSSM];

				UINT nSMF = pLight->GetShadowFrustumNumber();
				for (UINT iSMF = 0; iSMF < nSMF; ++iSMF)
				{
					ShadowMapFrustum* pSMF = pLight->GetShadowFrustumByIndex(iSMF);

					viwToLightProjArray[iSMF] = invView * pSMF->GetViewProjMatrix() * pSMF->GetTexScaleBiasMat();

					char pszTexShadowMap[MAX_PATH] = {0};
					snprintf(pszTexShadowMap, sizeof(pszTexShadowMap), "SamplerShadowMap%d", iSMF);
					m_pDefferedShadow->GetParameter(pszTexShadowMap)->setTexture( pSMF->GetDepthTexture() );
				}

				float fShadowMapSize = GetRenderSetting()->m_fShadowMapSize;
				Vector4 uShadowMapTexelSize(fShadowMapSize, 1.0f / fShadowMapSize, 0, 0);
				m_pDefferedShadow->GetParameter("u_shadowMapTexelSize")->setFloatArray(uShadowMapTexelSize,4);

				m_pDefferedShadow->GetParameter("viwToLightProjArray")->setMatrixArray(viwToLightProjArray,nSMF);
				m_pDefferedShadow->GetParameter("splitPos")->setFloatArray(pDirLight->GetSplitPos(),nSMF);

				ScreenQuad::Render(m_pDefferedShadow);

				break;
			}

		}

		GetRenderSystem()->SetRenderTarget(pPreTarget);
	}

	void DeferredShadowPass::ShoutDown()
	{

	}

}