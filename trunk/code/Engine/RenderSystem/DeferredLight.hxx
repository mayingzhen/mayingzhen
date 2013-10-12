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
		m_pDeferredEffect = NULL;
	}

	void DeferredLight::Init()
	{
		m_pDepthTex = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_R32F);
		m_pNormalTex = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		m_pDiffuse = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		m_pSpecular = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);

		m_pMaterDeferred = new Material();
		m_pDeferredEffect = new Effect("DeferredLight");
		m_pMaterDeferred->SetEffect(m_pDeferredEffect);

		Technique* pAmbientTech = m_pDeferredEffect->AddTechnique("AmbientLight","DeferredLight","AMBIENT_LIGHT");
		pAmbientTech->GetRenderState().m_bDepthWrite = false;

		Technique* pDirTech = m_pDeferredEffect->AddTechnique("DirectLight","DeferredLight","DIRECT_LIGHT");
		pDirTech->GetRenderState().m_bDepthWrite = false;
		pDirTech->GetRenderState().m_eBlendMode = BM_ADD;

		Technique* pPointTech = m_pDeferredEffect->AddTechnique("PointLight","DeferredLight","POINT_LIGHT");
		pPointTech->GetRenderState().m_bDepthWrite = false;
		pPointTech->GetRenderState().m_eBlendMode = BM_ADD;
	}

	void DeferredLight::GBufferPass()
	{
		GetRenderDevice()->PushRenderTarget(m_pDepthTex,0);
		GetRenderDevice()->PushRenderTarget(m_pNormalTex,1);

		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

		UINT nSolid = GetRenderQueue()->GetRenderObjNumber(RL_Solid);
		for (UINT i = 0; i < nSolid; ++i)
		{
			RenderObject* pSolidEntry = GetRenderQueue()->GetRenderObjByIndex(RL_Solid,i);
			Material* pMaterial = pSolidEntry->GetMaterial();
			Effect* pEffect = pMaterial->GetEffect();
			pEffect->SetCurCurTechnqiue("Gbuffer");
			pEffect->GetParameter("shininess")->setVector4(Vector4(6.0f,0,0,0)); 
			pSolidEntry->Render();

			/*pEffect->End();*/
		}

		GetRenderDevice()->PopRenderTarget(0);
		GetRenderDevice()->PopRenderTarget(1);
	}


	void DeferredLight::DefferedLighting()
	{
		GetRenderDevice()->PushRenderTarget(m_pDiffuse,0);
		GetRenderDevice()->PushRenderTarget(m_pSpecular,1);

		GetRenderDevice()->ClearBuffer(true,false,true,Color(0,0,0,0), 1.0f, 0);

		// AmbientLight
		m_pDeferredEffect->SetCurCurTechnqiue("AmbientLight");
 		Vector4 cAmbientColor = GetRenderSystem()->GetAmbientColor();
 		m_pDeferredEffect->GetParameter("light_color")->setVector4(cAmbientColor);
		ScreenQuad::Render(m_pMaterDeferred);	
		/*m_pDeferredEffect->End();*/

		Matrix4x4 matView = GetRenderSystem()->GetViewMatrix();

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

				m_pDeferredEffect->SetCurCurTechnqiue("DirectLight");
				m_pDeferredEffect->GetParameter("light_color")->setVector4(pLight->GetLightColor());

				Vector3 vDirWS = pDirLight->GetDirection();
				Vector3 vDirES;
				Vec3TransformNormal(&vDirES,&vDirWS,&matView);
				m_pDeferredEffect->GetParameter("light_dir_es")->setVector4(Vector4(vDirES.x,vDirES.y,vDirES.z,0));
				
				ScreenQuad::Render(m_pMaterDeferred);		

				/*m_pDeferredEffect->End();*/
			}
			else if (pLight->GetLightType() == LIGHT_POINT)
			{
				PointLight* pPointLight = (PointLight*)pLight;
				float fRadius = pPointLight->GetRadius();
				Vector3 vPos = pPointLight->GetPos();

				m_pDeferredEffect->SetCurCurTechnqiue("PointLight");
				m_pDeferredEffect->GetParameter("light_color")->setVector4(pLight->GetLightColor());

				Vector3 vPosES;
				Vec3TransformCoord(&vPosES,&vPos,&matView);
				m_pDeferredEffect->GetParameter("light_pos_es")->setVector4(Vector4(vPosES.x,vPosES.y,vPosES.z,1));	
				m_pDeferredEffect->GetParameter("light_radius")->setVector4(Vector4(fRadius,0,0,0));
							
				float cameraToCenter = Vec3Length(&vPosES);
				if (cameraToCenter < fRadius)
				{
					m_pDeferredEffect->GetCurTechnqiue()->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;	
				}
				else
				{
					m_pDeferredEffect->GetCurTechnqiue()->GetRenderState().m_eCullMode = CULL_FACE_SIDE_BACK;
				}

				
				UnitSphere::Render(m_pMaterDeferred,pPointLight->GetPos(),fRadius);

				/*m_pDeferredEffect->End();*/
			}

			GetMaterialManager()->SetCurLight(NULL);
		}


		GetRenderDevice()->PopRenderTarget(0);
		GetRenderDevice()->PopRenderTarget(1);
	}




}
