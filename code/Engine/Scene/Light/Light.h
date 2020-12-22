#pragma once


namespace ma
{
	class Camera;
	class PointLightProxy;

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
		void						SetLightColor(const ColourValue& cLightColor);

		void						SetLightIntensity(float fIntensity);
		float						GetLightIntensity() {return m_fLightIntensity;}

	private:
		LightProxy*					GetLightProxy();

		void						UpdateProxyColor();

	protected:
		LightType					m_eLightType;

		ColourValue					m_cLightColor;

		float						m_fLightIntensity;
	};


	class PointLight : public Light
	{
	
	public:
		PointLight();

		DECL_OBJECT(PointLight)

		float			GetRadius() {return m_fRadius;}

		void			SetRadius(float fRadius);

	private:
		PointLightProxy* GetPointLightProxy();

	private:

		float			m_fRadius;
	};

	RefPtr<PointLight> CreatePointLight();
	

	class SpotLight : public Light
	{
	
	public:
		SpotLight();

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



	class LightProxy : public RenderProxy
	{

	public:

		virtual void	Render(RenderQueue* pRenderQueue, RenderPass* pRenderPass);

	public:
		LightType		m_eLightType;

		Vector3			m_vLightColor[2];

		RefPtr<Technique>	m_pTech;
	};

	class DirLightProxy : public LightProxy
	{
	public:
		DirLightProxy();
	
	public:
		Vector3	m_vDir[2];
	};

	class PointLightProxy : public LightProxy
	{
	public:
		PointLightProxy();

		void SetWorldMatrix(const Matrix4& matws);

	public:
		
		Vector4 m_vPosRadius[2];

		RefPtr<Renderable>	m_pSphere;
	};


}
