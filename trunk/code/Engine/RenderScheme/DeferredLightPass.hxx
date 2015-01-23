#include "DeferredLightPass.h"
#include "../RenderSystem/RenderProfile.h"

namespace ma
{	
	DeferredLightPass::DeferredLightPass(Scene* pScene)
		:RenderPass(pScene)
	{

	}

	void DeferredLightPass::Init()
	{
		int nWidth = (int)m_pScene->GetViewport().width;
		int nHeight = (int)m_pScene->GetViewport().height;

		m_pDiffuse = GetRenderSystem()->CreateRenderTexture(nWidth, nHeight, PF_A8R8G8B8);
		m_pSpecular = GetRenderSystem()->CreateRenderTexture(nWidth, nHeight, PF_A8R8G8B8);

		m_pAmbientLight = CreateTechnique("AmbientLight","DeferredLight","DeferredLight","AMBIENT_LIGHT");
		m_pAmbientLight->GetRenderState().m_bDepthWrite = false;

		std::string strShaderDefine = "DIRECT_LIGHT";
		if ( GetRenderShadowCSM()->GetEnabled() )
		{
			strShaderDefine += ";SHADOW";
		}

		m_pDirLight = CreateTechnique("DirectLight","DeferredLight","DeferredLight",strShaderDefine.c_str());
		m_pDirLight->GetRenderState().m_bDepthWrite = false;
		m_pDirLight->GetRenderState().m_eBlendMode = BM_ADD;

		m_pPointLight = CreateTechnique("PointLight","DeferredLight","DeferredLight","POINT_LIGHT");
		m_pPointLight->GetRenderState().m_bDepthWrite = false;
		m_pPointLight->GetRenderState().m_eBlendMode = BM_ADD;
	}

	void DeferredLightPass::Render()
	{
		RENDER_PROFILE(DefferedLighting);

		RefPtr<Texture> pPreTarget0 = GetRenderSystem()->SetRenderTarget(m_pDiffuse,0); 
		RefPtr<Texture> pPreTarget1 = GetRenderSystem()->SetRenderTarget(m_pSpecular,1);

		GetRenderSystem()->ClearBuffer(true,false,true,ColourValue::Black, 1.0f, 0);

		// AmbientLight
		ColourValue cAmbientColor = GetLightSystem()->GetAmbientColor();
		m_pAmbientLight->SetParameter("light_color",Any(cAmbientColor));
		ScreenQuad::Render(m_pAmbientLight.get());	

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

				ScreenQuad::Render(m_pDirLight.get());		
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


				UnitSphere::Render(m_pPointLight.get(),pPointLight->GetPos(),fRadius);
			}

			GetRenderContext()->SetCurLight(NULL);
		}


		GetRenderSystem()->SetRenderTarget(pPreTarget0,0);
		GetRenderSystem()->SetRenderTarget(pPreTarget1,1);
	}

	void DeferredLightPass::ShoutDown()
	{
		m_pDiffuse = NULL;
		m_pSpecular = NULL;

		m_pDirLight = NULL;
		m_pPointLight = NULL;
		m_pAmbientLight = NULL;
	}

}

