#include "DeferredLight.h"

namespace ma
{
	static DeferredLight* gpDeferredLight = NULL;

	DeferredLight* GetDeferredLight()
	{
		return gpDeferredLight;
	}

	void SetDeferredLight(DeferredLight* pDeffLight)
	{
		gpDeferredLight = gpDeferredLight;
	}

	void DeferredLight::Init()
	{
		m_pDepthTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_R32F);
		m_pNormalTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);

		m_pDiffuse = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		m_pSpecular = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);

		m_pMaterDeferred = new Material("AMBIENT_LIGHT","DeferredLight");
	}

	void DeferredLight::DoRender()
	{
		GBufferPass();

		DefferedLighting();

		ShadingPass();
	}

	void DeferredLight::GBufferPass()
	{
		RenderTarget* pPreTar1 = GetRenderDevice()->SetRenderTarget(m_pDepthTex,0);
		RenderTarget* pPreTar2 = GetRenderDevice()->SetRenderTarget(m_pNormalTex,1);

		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

		int nSolidEntry = GetRenderSystem()->GetSolidEntryNumber();
		for (UINT i = 0; i < nSolidEntry; ++i)
		{
			Renderable* pSolidEntry = GetRenderSystem()->GetSolidEntryByIndex(i);
			Material* pMaterial = pSolidEntry->m_pMaterial;
			pMaterial->SetCurTechnqiue("gbuffer",NULL);
			GetRenderDevice()->DrawRenderable(pSolidEntry);
		}

		GetRenderDevice()->SetRenderTarget(pPreTar1,0);
		GetRenderDevice()->SetRenderTarget(pPreTar2,1);
	}


	void DeferredLight::DefferedLighting()
	{
		RenderTarget* pPreTar1 = GetRenderDevice()->SetRenderTarget(m_pDiffuse,0);
		RenderTarget* pPreTar2 = GetRenderDevice()->SetRenderTarget(m_pSpecular,1);

		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);


		ScreenQuad::Render(m_pMaterDeferred);	

		UINT nLigtNumber = GetRenderSystem()->GetLightNumber();
		for (UINT i = 0; i < nLigtNumber; ++i)
		{
			Light* pLight = GetRenderSystem()->GetLightByIndex(i);
			if (pLight == NULL)
				continue;

			GetMaterialManager()->SetCurLight(pLight);
	
			if (pLight->GetLightType() == LIGHT_DIRECTIONAL)
			{
				//m_pMaterDeferred->SetCurTechnqiue("DirectLight");

				//m_pMaterDeferred->GetParameter("");
				ScreenQuad::Render(m_pMaterDeferred);		
			}
			else if (pLight->GetLightType() == LIGHT_POINT || pLight->GetLightType() == LIGHT_SPOT)
			{
				//Vector3 camePos = m_pCamera->GetTransform().m_vPos;
				//if (camePos.InLight)
				//{
				//	
				//}

				//m_pMaterDeferred->SetCurTechnqiue("DirectLight");
				//Material::SetAutoBingLight(pLight);
				//m_pMaterDeferred->GetParameter("");
			}

			//Material::SetAutoBingLight(NULL);
		}


		GetRenderDevice()->SetRenderTarget(pPreTar1,0);
		GetRenderDevice()->SetRenderTarget(pPreTar2,1);
	}


	void DeferredLight::ShadingPass()
	{
		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

		int nSolidEntry = GetRenderSystem()->GetSolidEntryNumber();
		for (UINT i = 0; i < nSolidEntry; ++i)
		{
			Renderable* pSolidEntry = GetRenderSystem()->GetSolidEntryByIndex(i);

			Material* pMaterial = pSolidEntry->m_pMaterial;
			pMaterial->SetCurTechnqiue("default","DeferredLight");
			GetRenderDevice()->DrawRenderable(pSolidEntry);
		}
	}


}
