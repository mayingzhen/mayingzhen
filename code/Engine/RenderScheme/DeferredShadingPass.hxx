#include "DeferredShadingPass.h"
#include "../RenderSystem/RenderProfile.h"

namespace ma
{	
	DeferredShadingPass::DeferredShadingPass(Scene* pScene)
		:RenderPass(pScene)
	{

	}

	void DeferredShadingPass::Init()
	{
		m_pAmbientLight = CreateTechnique("AmbientLight","DeferredLight","DeferredLight","AMBIENT_LIGHT");
		RefPtr<DepthStencilState> pDSState = CreateDepthStencilState();
		pDSState->m_bDepthWrite = false;
		m_pAmbientLight->SetDepthStencilState(pDSState.get());

		m_pDirLight = CreateTechnique("DirectLight","DeferredLight","DeferredLight","DIRECT_LIGHT");
		m_pDirLight->SetDepthStencilState(pDSState.get());

		RefPtr<BlendState> pBlendState = CreateBlendState();
		pBlendState->m_eBlendMode = BM_ADD;
		m_pDirLight->SetBlendState(pBlendState.get());

		m_pPointLight = CreateTechnique("PointLight","DeferredLight","DeferredLight","POINT_LIGHT");
		m_pPointLight->SetDepthStencilState(pDSState.get());
		m_pDirLight->SetBlendState(pBlendState.get());
	}

	void DeferredShadingPass::Reset()
	{

	}

	void DeferredShadingPass::Render()
	{
		RENDER_PROFILE(DefferedLighting);

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::Black, 1.0f, 0);

		// AmbientLight
		Vector3 cAmbientColor = m_pScene->GetAmbientColor();
		m_pAmbientLight->SetParameter("light_color",Any(cAmbientColor));
		ScreenQuad::Render(m_pAmbientLight.get());	

		Matrix4 matView = GetRenderContext()->GetViewMatrix();

		UINT nLigtNumber = m_pScene->GetVisibleLightNum();
		for (UINT i = 0; i < nLigtNumber; ++i)
		{
			Light* pLight = m_pScene->GetVisibleLightByIndex(i);
			if (pLight == NULL)
				continue;

			GetRenderContext()->SetCurLight(pLight);

			if (pLight->GetLightType() == LIGHT_DIRECTIONAL)
			{       
				DirectonalLight* pDirLight = (DirectonalLight*)pLight;
				
				Uniform* pUniformDirES = m_pDirLight->GetUniform("light_dir_es");
				Uniform* pUniformColor = m_pDirLight->GetUniform("light_color");
					
				Quaternion qRotate = matView.extractQuaternion();
				Vector3 vDirES = qRotate * -pDirLight->GetSceneNode()->GetForward();
				
				m_pDirLight->SetValue(pUniformDirES,vDirES);
				m_pDirLight->SetValue(pUniformColor,pLight->GetLightColor());

				ScreenQuad::Render(m_pDirLight.get());		
			}
			else if (pLight->GetLightType() == LIGHT_POINT)
			{
				PointLight* pPointLight = (PointLight*)pLight;
	
				Uniform* pUniformColor = m_pPointLight->GetUniform("light_color");
				Uniform* pUniformPosESRadius = m_pPointLight->GetUniform("light_pos_es_radius");
				
				Vector3 vPosES = matView * pPointLight->GetSceneNode()->GetPos();
				Vector4 vPosESRadius(vPosES.x,vPosES.y,vPosES.z,pPointLight->GetRadius());
				
				m_pPointLight->SetValue(pUniformColor,pLight->GetLightColor());
				m_pPointLight->SetValue(pUniformPosESRadius,vPosESRadius);
		
				float cameraToCenter = vPosES.length();
				if (cameraToCenter < pPointLight->GetRadius())
				{
					//m_pPointLight->m_eCullMode = CULL_FACE_SIDE_FRONT;	
				}
				else
				{
					//m_pPointLight->m_eCullMode = CULL_FACE_SIDE_BACK;
				}

				UnitSphere::Render(m_pPointLight.get(),pPointLight->GetSceneNode()->GetPos(),pPointLight->GetRadius());
			}

			GetRenderContext()->SetCurLight(NULL);
		}
	}

	void DeferredShadingPass::Shoutdown()
	{
		m_pDirLight = NULL;
		m_pPointLight = NULL;
		m_pAmbientLight = NULL;
	}

}

