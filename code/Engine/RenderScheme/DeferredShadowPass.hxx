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

		//m_bHWShadowMap = GetRenderDevice()->CheckTextureFormat(FMT_D24S8,USAGE_DEPTHSTENCIL);

		std::string strShaderDefine = "SCEERN_LIGHT";
		if ( GetDeviceCapabilities()->GetDepthTextureSupported() )
		{
			strShaderDefine += ";HWPCF";
		}

		m_pDefferedShadow = CreateTechnique("DeferredShadow","DefferedShadow","DefferedShadow",strShaderDefine.c_str());

		m_pDefferedShadow->GetRenderState().m_bDepthWrite = false;
	}

	void DeferredShadowPass::Render()
	{
		RENDER_PROFILE(DeferredShadowPass);

		RefPtr<Texture> pPreTarget = GetRenderSystem()->SetRenderTarget(m_pShadowTex);

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::Black,1,0);

		Matrix4 invView = GetRenderContext()->GetViewMatrix().inverse();

		UINT nLight = GetLightSystem()->GetLightNumber();
		for (UINT iLight = 0; iLight < nLight; ++iLight)
		{
			Light* pLight = GetLightSystem()->GetLightByIndex(iLight);
			//if ( !pLight->IsCreateShadow() )
			//	continue;

			if (pLight->GetLightType() == LIGHT_DIRECTIONAL)
			{
				DirectonalLight* pDirLight = (DirectonalLight*)pLight;

//				Matrix4 viwToLightProjArray[DirectonalLight::NUM_PSSM];

// 				UINT nSMF = pLight->GetShadowFrustumNumber();
// 				for (UINT iSMF = 0; iSMF < nSMF; ++iSMF)
// 				{
// 					ShadowMapFrustum* pSMF = pLight->GetShadowFrustumByIndex(iSMF);
// 
// 					viwToLightProjArray[iSMF] = pSMF->GetTexScaleBiasMat() * pSMF->GetViewProjMatrix() * invView;
// 
// 					//char pszTexShadowMap[MAX_PATH] = {0};
// 					//snprintf(pszTexShadowMap, sizeof(pszTexShadowMap), "SamplerShadowMap%d", iSMF);
// 					//m_pDefferedShadow->SetParameter(pszTexShadowMap,Any(pSMF->GetDepthTexture()));
// 				}

				//float fShadowMapSize = GetRenderSetting()->m_fShadowMapSize;
				//Vector4 uShadowMapTexelSize(fShadowMapSize, 1.0f / fShadowMapSize, 0, 0);
				//m_pDefferedShadow->SetParameter("u_shadowMapTexelSize",Any(uShadowMapTexelSize));

				ASSERT(false);
				//m_pDefferedShadow->GetParameter("viwToLightProjArray")->setMatrixArray(viwToLightProjArray,nSMF);
				//m_pDefferedShadow->GetParameter("splitPos")->setFloatArray(pDirLight->GetSplitPos(),nSMF);

				ScreenQuad::Render(m_pDefferedShadow.get());

				break;
			}

		}

		GetRenderSystem()->SetRenderTarget(pPreTarget);
	}

	void DeferredShadowPass::ShoutDown()
	{

	}

}