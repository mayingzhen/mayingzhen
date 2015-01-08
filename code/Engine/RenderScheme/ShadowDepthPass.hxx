#include "ShadowDepthPass.h"

namespace ma
{
	IMPL_OBJECT(ShadowDepthPass,RenderPass);

	void ShadowDepthPass::Init()
	{
		GetRenderSetting()->m_bShadow = true;
	}

	void ShadowDepthPass::Render()
	{
		RENDER_PROFILE(ShadowDepthPass);

		Matrix4 matView = GetRenderContext()->GetViewMatrix();
		Matrix4 matProj = GetRenderContext()->GetProjMatrix();

		UINT nLight = GetLightSystem()->GetLightNumber();
		for (UINT iLight = 0; iLight < nLight; ++iLight)
		{
			Light* pLight = GetLightSystem()->GetLightByIndex(iLight);
			if ( !pLight->IsCreateShadow() )
				continue;

// 			UINT nSMF = pLight->GetShadowFrustumNumber();
// 			for (UINT iSMF = 0; iSMF < nSMF; ++iSMF)
// 			{
// 				ShadowMapFrustum* pSMF = pLight->GetShadowFrustumByIndex(iSMF);
// 				pSMF->RenderShadowMap();
// 			}
		}

		GetRenderContext()->SetViewMatrix(matView);
		GetRenderContext()->SetProjMatrix(matProj);
	}

	void ShadowDepthPass::ShoutDown()
	{

	}

}

