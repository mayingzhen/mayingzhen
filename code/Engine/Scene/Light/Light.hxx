#include "Light.h"
#include "Scene/Scene.h"


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

	void PointLight::SetRadius(float fRadius) 
	{
		m_fRadius = fRadius ? fRadius : 1.0f;

		SetAABB( AABB(Vector3(-m_fRadius),Vector3(m_fRadius)) );
	}

	void PointLight::Show(Camera* pCamera)
	{
		m_pSceneNode->GetScene()->AddRenderLight(this);
	}

	RefPtr<PointLight> CreatePointLight()
	{
		return new PointLight;
	}

}



