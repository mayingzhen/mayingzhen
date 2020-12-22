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


	void Light::SetLightColor(const ColourValue& cLightColor)
	{
		m_cLightColor = cLightColor;

		UpdateProxyColor();
	}

	void Light::SetLightIntensity(float fIntensity)
	{
		m_fLightIntensity = fIntensity;

		UpdateProxyColor();
	}

	void Light::UpdateProxyColor()
	{
		Vector3 vColor = Vector3(m_cLightColor.r, m_cLightColor.g, m_cLightColor.b);
		vColor *= m_fLightIntensity;
		LightProxy* proxy = GetLightProxy();
		proxy->m_vLightColor[GetRenderSystem()->CurThreadFill()] = vColor;
	}

	LightProxy* Light::GetLightProxy()
	{
		LightProxy* pProxy = dynamic_cast<LightProxy*>(m_pRenderproxy.get());
		ASSERT(pProxy);
		return pProxy;
	}

	PointLight::PointLight()
	{
		m_eLightType = LIGHT_POINT;

		m_pRenderproxy = new PointLightProxy();
	}

	PointLightProxy* PointLight::GetPointLightProxy()
	{
		PointLightProxy* pProxy = dynamic_cast<PointLightProxy*>(m_pRenderproxy.get());
		ASSERT(pProxy);
		return pProxy;
	}

	void PointLight::SetRadius(float fRadius) 
	{
		m_fRadius = fRadius ? fRadius : 1.0f;

		SetAABB( AABB(Vector3(-m_fRadius),Vector3(m_fRadius)) );

		Matrix4 matTras = Matrix4::IDENTITY;
		matTras.setTrans( this->GetSceneNode()->GetPosWS() );
		matTras.setScale(Vector3(m_fRadius, m_fRadius, m_fRadius));
		GetPointLightProxy()->SetWorldMatrix(matTras);

		GetPointLightProxy()->m_vPosRadius[2] = Vector4(this->GetSceneNode()->GetPosWS(), fRadius);
	}

	RefPtr<PointLight> CreatePointLight()
	{
		return new PointLight;
	}

	SpotLight::SpotLight()
	{

	}

	void LightProxy::Render(RenderQueue* pRenderQueue, RenderPass* pRenderPass)
	{
		pRenderQueue->AddLightObj(this);
	}


	DirLightProxy::DirLightProxy()
	{
		m_eLightType = LIGHT_DIRECTIONAL;

		if (GetRenderSystem()->GetDefferedLightRenderPass())
		{
			m_pTech = CreateTechnique("shader/dirlight.tech", "", GetRenderSystem()->GetDefferedLightRenderPass());
		}
	}

	PointLightProxy::PointLightProxy()
	{
		m_eLightType = LIGHT_POINT;

		if (GetRenderSystem()->GetDefferedLightRenderPass())
		{
			m_pTech = CreateTechnique("shader/pointlight.tech", "", GetRenderSystem()->GetDefferedLightRenderPass());

			m_pSphere = UnitSphere::CreateRenderable();
		}
	}

	void PointLightProxy::SetWorldMatrix(const Matrix4& matws)
	{
		if (m_pSphere)
		{
			m_pSphere->SetWorldMatrix(matws);
		}
	}

}



