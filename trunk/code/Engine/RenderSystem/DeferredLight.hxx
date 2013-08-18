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
		gpDeferredLight = pDeffLight;
	}

	DeferredLight::DeferredLight()
	{
		m_pDepthTex = NULL;
		m_pNormalTex = NULL;
		m_pDiffuse = NULL;
		m_pSpecular = NULL;
		m_pMaterDeferred = NULL;
	}

	void DeferredLight::Init()
	{
		m_pDepthTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_R32F);
		m_pNormalTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);

		m_pDiffuse = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		m_pSpecular = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);

		m_pMaterDeferred = new Material(NULL,NULL);
	}

	void DeferredLight::DoRender()
	{
		GBufferPass();

		DefferedLighting();

		//ShadingPass();
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
			pMaterial->GetParameter("shininess")->setVector4(Vector4(6.0f,0,0,0)); 
			GetRenderDevice()->DrawRenderable(pSolidEntry);
		}

		GetRenderDevice()->SetRenderTarget(pPreTar1,0);
		GetRenderDevice()->SetRenderTarget(pPreTar2,1);
	}


	void DeferredLight::DefferedLighting()
	{
		RenderTarget* pPreTar1 = GetRenderDevice()->SetRenderTarget(m_pDiffuse,0);
		RenderTarget* pPreTar2 = GetRenderDevice()->SetRenderTarget(m_pSpecular,1);

		GetRenderDevice()->ClearBuffer(true,false,true,Color(0,0,0,0), 1.0f, 0);

		// AmbientLight
		m_pMaterDeferred->SetCurTechnqiue("DeferredLight","AMBIENT_LIGHT");
		Vector4 cAmbientColor = GetRenderSystem()->GetAmbientColor();
		m_pMaterDeferred->GetParameter("light_color")->setVector4(cAmbientColor);
		m_pMaterDeferred->GetCurTechnqiue()->GetRenderState().m_bDepthWrite = false;
		ScreenQuad::Render(m_pMaterDeferred);	

		Matrix4x4 matView = GetRenderSystem()->GetCamera()->GetViewMatrix();

		UINT nLigtNumber = GetRenderSystem()->GetLightNumber();
		for (UINT i = 0; i < nLigtNumber; ++i)
		{
			Light* pLight = GetRenderSystem()->GetLightByIndex(i);
			if (pLight == NULL)
				continue;

			GetMaterialManager()->SetCurLight(pLight);
	
			if (pLight->GetLightType() == LIGHT_DIRECTIONAL)
			{
				DirectonalLight* pDirLight = (DirectonalLight*)pLight;

				m_pMaterDeferred->SetCurTechnqiue("DeferredLight","DIRECT_LIGHT");
				m_pMaterDeferred->GetParameter("light_color")->setVector4(pLight->GetLightColor());

				Vector3 vDirWS = pDirLight->GetDirection();
				Vector3 vDirES;
				Vec3TransformNormal(&vDirES,&vDirWS,&matView);
				m_pMaterDeferred->GetParameter("light_dir_es")->setVector4(Vector4(vDirES.x,vDirES.y,vDirES.z,0));
				
				m_pMaterDeferred->GetCurTechnqiue()->GetRenderState().m_bDepthWrite = false;
				m_pMaterDeferred->GetCurTechnqiue()->GetRenderState().m_eBlendMode = BM_ADD;
				ScreenQuad::Render(m_pMaterDeferred);		
			}
			else if (pLight->GetLightType() == LIGHT_POINT)
			{
				PointLight* pPointLight = (PointLight*)pLight;
				float fRadius = pPointLight->GetRadius();
				Vector3 vPos = pPointLight->GetPos();


				m_pMaterDeferred->SetCurTechnqiue("DeferredLight","POINT_LIGHT");
				m_pMaterDeferred->GetParameter("light_color")->setVector4(pLight->GetLightColor());

				Vector3 vPosES;
				Vec3TransformCoord(&vPosES,&vPos,&matView);
				m_pMaterDeferred->GetParameter("light_pos_es")->setVector4(Vector4(vPosES.x,vPosES.y,vPosES.z,1));	
				m_pMaterDeferred->GetParameter("light_radius")->setVector4(Vector4(fRadius,0,0,0));
	
				m_pMaterDeferred->GetCurTechnqiue()->GetRenderState().m_bDepthWrite = false;
				m_pMaterDeferred->GetCurTechnqiue()->GetRenderState().m_eBlendMode = BM_ADD;
							
				float cameraToCenter = Vec3Length(&vPosES);
				if (cameraToCenter < fRadius)
				{
					m_pMaterDeferred->GetCurTechnqiue()->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;	
				}
				else
				{
					m_pMaterDeferred->GetCurTechnqiue()->GetRenderState().m_eCullMode = CULL_FACE_SIDE_BACK;
				}

				
				UnitSphere::Render(m_pMaterDeferred,pPointLight->GetPos(),fRadius);
			}

			GetMaterialManager()->SetCurLight(NULL);
		}


		GetRenderDevice()->SetRenderTarget(pPreTar1,0);
		GetRenderDevice()->SetRenderTarget(pPreTar2,1);
	}




}
