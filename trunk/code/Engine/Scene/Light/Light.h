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



	class RENDER_API Light : public RenderComponent
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

		virtual void				UpdateShadowFrustum(Camera* pCamera) {}

	protected:
		LightType					m_eLightType;

		ColourValue					m_cLightColor;

		float						m_fLightIntensity;
	};

	class RENDER_API PointLight : public Light
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
	
	class RENDER_API DirectonalLight : public Light
	{
		DECL_OBJECT(DirectonalLight)

	public:
		DirectonalLight();

		Vector3			GetDirection();

	};


	class RENDER_API SpotLight : public Light
	{
		DECL_OBJECT(SpotLight)

	public:
		SpotLight()	{m_eLightType = LIGHT_SPOT;}

		Transform		GetTransform() {return m_tsfWS;}

		void			SetTransform(const Transform& tsfWS) {m_tsfWS = tsfWS;}

		//void			LookAt(const Vector3& vEye,const Vector3& vAt,const Vector3& vUp);	

		Matrix4			GetViewMatrix() {return m_mView;}

		Matrix4			GetProjmatrix()	{return m_mProj;}

	private:
		Transform		m_tsfWS;

		float			m_fAspectRatio;
		float			m_fNearClip;
		float			m_fFarClip;
		float			m_fFOV;

		Vector3			m_vTarget;
		Vector3			m_vUpVector;
		Matrix4			m_mView;
		Matrix4			m_mProj;
	};


	DeclareRefPtr(PointLight);
	DeclareRefPtr(DirectonalLight);

}

#endif


