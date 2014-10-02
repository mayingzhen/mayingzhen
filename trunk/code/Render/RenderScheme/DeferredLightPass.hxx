#include "DeferredLightPass.h"
#include "../RenderSystem/RenderProfile.h"

namespace ma
{	
	static DeferredLightPass* gpDeferredLightPass = NULL;

	DeferredLightPass* GetDeferredLightPass() {return gpDeferredLightPass;}

	IMPL_OBJECT(DeferredLightPass,RenderPass);

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

		Texture* pPreTarget0 = GetRenderSystem()->SetRenderTarget(m_pDiffuse,0); 
		Texture* pPreTarget1 = GetRenderSystem()->SetRenderTarget(m_pSpecular,1);

		GetRenderSystem()->ClearBuffer(true,false,true,ColourValue::White, 1.0f, 0);

		// AmbientLight
		ColourValue cAmbientColor = GetLightSystem()->GetAmbientColor();
		m_pAmbientLight->SetParameter("light_color",Any(cAmbientColor));
		ScreenQuad::Render(m_pAmbientLight);	

		Matrix4 matView = GetRenderContext()->GetViewMatrix();

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

				m_pDirLight->SetParameter("light_color",Any(pLight->GetLightColor()));
			
				Vector3 vDirES = matView * pDirLight->GetDirection();
				m_pDirLight->SetParameter("light_dir_es",Any(vDirES));

				ScreenQuad::Render(m_pDirLight);		
			}
			else if (pLight->GetLightType() == LIGHT_POINT)
			{
				PointLight* pPointLight = (PointLight*)pLight;
	
				m_pPointLight->SetParameter("light_color",Any(pLight->GetLightColor()));

				Vector3 vPosES = matView * pPointLight->GetPos();
				float fRadius = pPointLight->GetRadius();
				m_pPointLight->SetParameter("light_pos_es",Any(vPosES));
				m_pPointLight->SetParameter("light_radius",Any(fRadius));

				float cameraToCenter = vPosES.length();
				if (cameraToCenter < fRadius)
				{
					m_pPointLight->GetRenderState().m_eCullMode = CULL_FACE_SIDE_BACK;	
				}
				else
				{
					m_pPointLight->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;
				}


				UnitSphere::Render(m_pPointLight,pPointLight->GetPos(),fRadius);
			}

			GetRenderContext()->SetCurLight(NULL);
		}


		GetRenderSystem()->SetRenderTarget(pPreTarget0,0);
		GetRenderSystem()->SetRenderTarget(pPreTarget1,1);
	}

	void DeferredLightPass::ShoutDown()
	{

	}

}

