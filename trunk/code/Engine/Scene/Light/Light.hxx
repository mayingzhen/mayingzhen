#include "Light.h"
#include "LightSystem.h"


namespace ma
{
	IMPL_OBJECT(Light,RenderComponent)

	Light::Light()
	{
		m_cLightColor = ColourValue::White;
		m_fLightIntensity = 1.0f;
	
		GetLightSystem()->AddLight(this);
	}

	Light::~Light()
	{
		GetLightSystem()->RemoveLight(this);
	}


	IMPL_OBJECT(PointLight,Light)
	
	Vector3 PointLight::GetPos()
	{
		return m_pSceneNode->GetTransformWS().m_vPos;
	}

	IMPL_OBJECT(SpotLight,Light)

	
	IMPL_OBJECT(DirectonalLight,Light)

	DirectonalLight::DirectonalLight()
	{
		m_eLightType = LIGHT_DIRECTIONAL;
	}

	Vector3 DirectonalLight::GetDirection()
	{
		return m_pSceneNode->GetForward();
	}
}



