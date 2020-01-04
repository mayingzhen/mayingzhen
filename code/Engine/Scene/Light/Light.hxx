#include "Light.h"


namespace ma
{
	Light::Light()
	{
		m_cLightColor = ColourValue::White;
		m_fLightIntensity = 1.0f;
	}

	Light::~Light()
	{
	}

	void Light::Render(RenderQueue* pRenderQueue)
	{
		pRenderQueue->AddLight(this,m_pTech.get());
	}

	PointLight::PointLight()
	{
		if (GetRenderSystem()->GetDefferedLightRenderPass())
		{
			m_pTech = CreateTechnique("shader/pointlight.tech", "", GetRenderSystem()->GetDefferedLightRenderPass());
		}
		
		m_eLightType = LIGHT_POINT;
	}

	void PointLight::SetRadius(float fRadius) 
	{
		m_fRadius = fRadius ? fRadius : 1.0f;

		SetAABB( AABB(Vector3(-m_fRadius),Vector3(m_fRadius)) );
	}

	RefPtr<PointLight> CreatePointLight()
	{
		return new PointLight;
	}


	SpotLight::SpotLight()
	{

	}

}



