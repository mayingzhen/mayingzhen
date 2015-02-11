#ifndef  _Light__H__
#define  _Light__H__


namespace ma
{
	class Camera;

	enum LightType
	{
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,			
		LIGHT_POINT,	
	};



	class Light : public Component
	{
		DECL_OBJECT(Light)

	public:
		Light();

		~Light();

		LightType					GetLightType() {return m_eLightType;}

		const ColourValue&			GetLightColor() {return m_cLightColor;}
		void						SetLightColor(const ColourValue& cLightColor) {m_cLightColor = cLightColor;}

		void						SetLightIntensity(float fIntensity) {m_fLightIntensity = fIntensity;}
		float						GetLightIntensity() {return m_fLightIntensity;}

	protected:
		LightType					m_eLightType;

		ColourValue					m_cLightColor;

		float						m_fLightIntensity;
	};

	class PointLight : public Light
	{
		DECL_OBJECT(PointLight)

	public:
		PointLight() {m_eLightType = LIGHT_POINT;}
		
		Vector3			GetPos();

		float			GetRadius() {return m_fRadius;}

		void			SetRadius(float fRadius) {m_fRadius = fRadius ? fRadius : 1.0f;}

	private:

		float		m_fRadius;
	};
	
	class DirectonalLight : public Light
	{
		DECL_OBJECT(DirectonalLight)

	public:
		DirectonalLight();

		Vector3			GetDirection();

	};


	class SpotLight : public Light
	{
		DECL_OBJECT(SpotLight)

	public:
		SpotLight()	{m_eLightType = LIGHT_SPOT;}

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

#endif


