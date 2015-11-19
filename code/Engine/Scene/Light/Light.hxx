#include "Light.h"
#include "LightSystem.h"


namespace ma
{
	Light::Light()
	{
		m_cLightColor = ColourValue::White;
		m_fLightIntensity = 1.0f;
	
		//GetLightSystem()->AddLight(this);
	}

	Light::~Light()
	{
		//GetLightSystem()->RemoveLight(this);
	}

	DirectonalLight::DirectonalLight()
	{
		m_eLightType = LIGHT_DIRECTIONAL;
	}

	RefPtr<PointLight> CreatePointLight()
	{
		return new PointLight;
	}

}



