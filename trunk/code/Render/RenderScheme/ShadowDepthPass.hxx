#include "ShadowDepthPass.h"

namespace ma
{

	void ShadowDepthPass::Init()
	{

	}

	void ShadowDepthPass::Render()
	{
		RENDER_PROFILE(ShadowDepthPass);

		Matrix4x4 matView = GetRenderContext()->GetViewMatrix();
		Matrix4x4 matProj = GetRenderContext()->GetProjMatrix();

		UINT nLight = GetLightSystem()->GetLightNumber();
		for (UINT iLight = 0; iLight < nLight; ++iLight)
		{
			Light* pLight = GetLightSystem()->GetLightByIndex(iLight);
			if ( !pLight->IsCreateShadow() )
				continue;

			UINT nSMF = pLight->GetShadowFrustumNumber();
			for (UINT iSMF = 0; iSMF < nSMF; ++iSMF)
			{
				ShadowMapFrustum* pSMF = pLight->GetShadowFrustumByIndex(iSMF);
				pSMF->RenderShadowMap();
			}
		}

		GetRenderContext()->SetViewMatrix(matView);
		GetRenderContext()->SetProjMatrix(matProj);
	}

	void ShadowDepthPass::ShoutDown()
	{

	}

}

