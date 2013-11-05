#include "DeferredLightPass.h"
#include "../RenderSystem/RenderProfile.h"

namespace ma
{	
	static DeferredLightPass* gpDeferredLightPass = NULL;

	DeferredLightPass* GetDeferredLightPass() {return gpDeferredLightPass;}

	void DeferredLightPass::Init()
	{
		gpDeferredLightPass = this;

		m_pDiffuse = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		m_pSpecular = GetRenderSystem()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);

		m_pAmbientLight = new Technique("AmbientLight","DeferredLight","DeferredLight","AMBIENT_LIGHT");
		m_pAmbientLight->GetRenderState().m_bDepthWrite = false;

		std::string strShaderDefine = "DIRECT_LIGHT";
		if ( GetRenderSetting()->m_bShadow )
		{
			strShaderDefine += ";SHADOW";
		}
		m_pDirLight = new Technique("DirectLight","DeferredLight","DeferredLight",strShaderDefine.c_str());
		m_pDirLight->GetRenderState().m_bDepthWrite = false;
		m_pDirLight->GetRenderState().m_eBlendMode = BM_ADD;

		m_pPointLight = new Technique("PointLight","DeferredLight","DeferredLight","POINT_LIGHT");
		m_pPointLight->GetRenderState().m_bDepthWrite = false;
		m_pPointLight->GetRenderState().m_eBlendMode = BM_ADD;
	}

	void DeferredLightPass::Render()
	{
		RENDER_PROFILE(DefferedLighting);

		GetRenderDevice()->PushRenderTarget(m_pDiffuse,0);
		GetRenderDevice()->PushRenderTarget(m_pSpecular,1);

		GetRenderDevice()->ClearBuffer(true,false,true,Color(0,0,0,0), 1.0f, 0);

		// AmbientLight
		Vector4 cAmbientColor = GetLightSystem()->GetAmbientColor();
		m_pAmbientLight->GetParameter("light_color")->setVector4(cAmbientColor);
		ScreenQuad::Render(m_pAmbientLight);	

		Matrix4x4 matView = GetRenderContext()->GetViewMatrix();

		UINT nLigtNumber = GetLightSystem()->GetLightNumber();
		for (UINT i = 0; i < nLigtNumber; ++i)
		{
			Light* pLight = GetLightSystem()->GetLightByIndex(i);
			if (pLight == NULL)
				continue;

			GetRenderContext()->SetCurLight(pLight);

			if (pLight->GetLightType() == LIGHT_DIRECTIONAL)
			{
				DirectonalLight* pDirLight = (DirectonalLight*)pLight;

				m_pDirLight->GetParameter("light_color")->setVector4(pLight->GetLightColor());

				Vector3 vDirWS = pDirLight->GetDirection();
				Vector3 vDirES;
				Vec3TransformNormal(&vDirES,&vDirWS,&matView);
				m_pDirLight->GetParameter("light_dir_es")->setVector4(Vector4(vDirES.x,vDirES.y,vDirES.z,0));

				ScreenQuad::Render(m_pDirLight);		
			}
			else if (pLight->GetLightType() == LIGHT_POINT)
			{
				PointLight* pPointLight = (PointLight*)pLight;
				float fRadius = pPointLight->GetRadius();
				Vector3 vPos = pPointLight->GetPos();

				m_pPointLight->GetParameter("light_color")->setVector4(pLight->GetLightColor());

				Vector3 vPosES;
				Vec3TransformCoord(&vPosES,&vPos,&matView);
				m_pPointLight->GetParameter("light_pos_es")->setVector4(Vector4(vPosES.x,vPosES.y,vPosES.z,1));	
				m_pPointLight->GetParameter("light_radius")->setVector4(Vector4(fRadius,0,0,0));

				float cameraToCenter = Vec3Length(&vPosES);
				if (cameraToCenter < fRadius)
				{
					m_pPointLight->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;	
				}
				else
				{
					m_pPointLight->GetRenderState().m_eCullMode = CULL_FACE_SIDE_BACK;
				}


				UnitSphere::Render(m_pPointLight,pPointLight->GetPos(),fRadius);
			}

			GetRenderContext()->SetCurLight(NULL);
		}


		GetRenderDevice()->PopRenderTarget(0);
		GetRenderDevice()->PopRenderTarget(1);
	}

	void DeferredLightPass::ShoutDown()
	{

	}

}

