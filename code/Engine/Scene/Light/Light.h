#pragma once


namespace ma
{
	class Camera;

	enum LightType
	{
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,			
		LIGHT_POINT,	
		LIGHT_AMBIENT,
	};



	class Light : public RenderComponent
	{
	
	public:
		Light();

		~Light();

		DECL_OBJECT(Light)

		LightType					GetLightType() {return m_eLightType;}

		const ColourValue&			GetLightColor() const {return m_cLightColor;}
		void						SetLightColor(const ColourValue& cLightColor) {m_cLightColor = cLightColor;}

		void						SetLightIntensity(float fIntensity) {m_fLightIntensity = fIntensity;}
		float						GetLightIntensity() {return m_fLightIntensity;}

		void						Render(RenderQueue* pRenderQueue);

	protected:
		LightType					m_eLightType;

		ColourValue					m_cLightColor;

		float						m_fLightIntensity;
	};

	class PointLight : public Light
	{
	
	public:
		PointLight() {m_eLightType = LIGHT_POINT;}

		DECL_OBJECT(PointLight)

		float			GetRadius() {return m_fRadius;}

		void			SetRadius(float fRadius);

	private:

		float			m_fRadius;
	};

	RefPtr<PointLight> CreatePointLight();
	

	class SpotLight : public Light
	{
	
	public:
		SpotLight()	{m_eLightType = LIGHT_SPOT;}

		DECL_OBJECT(SpotLight)

		Matrix4			GetViewMatrix() {return m_mView;}

		Matrix4			GetProjmatrix()	{return m_mProj;}

	private:

		float			m_fAspectRatio;
		float			m_fNearClip;
		float			m_fFarClip;
		float			m_fFOV;

		Vector3			m_vTarget;
		Vector3			m_vUpVector;
		Matrix4			m_mView;
		Matrix4			m_mProj;
	};

}
