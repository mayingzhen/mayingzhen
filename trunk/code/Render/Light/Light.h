#ifndef  _Light__H__
#define  _Light__H__



namespace ma
{
	class ShadowMapFrustum;
	class Camera;

	enum LightType
	{
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,			
		LIGHT_POINT,	
	};


	class RENDER_API Light : public Component
	{
		DECL_OBJECT(Light)

	public:
		Light(GameObject* pGameObj);

		~Light();

		LightType					GetLightType() {return m_eLightType;}

		const Vector4&				GetLightColor() {return m_cLightColor;}

		void						SetLightColor(const Vector4& cLightColor) {m_cLightColor = cLightColor;}

		void						SetLightIntensity(float fIntensity) {m_fLightIntensity = fIntensity;}

		float						GetLightIntensity() {return m_fLightIntensity;}

		bool						IsCreateShadow() {return m_bCreateShadow;}

		virtual void				SetCreateShadow(bool bCreateShaow) {m_bCreateShadow = bCreateShaow;}

		virtual void				UpdateShadowFrustum(Camera* pCamera) {}

		UINT						GetShadowFrustumNumber() {return m_arrShadowFrustum.size();}

		ShadowMapFrustum*			GetShadowFrustumByIndex(UINT index) {return m_arrShadowFrustum[index];}

	protected:
		LightType					m_eLightType;

		Vector4						m_cLightColor;

		float						m_fLightIntensity;

		bool						m_bCreateShadow;

		std::vector<ShadowMapFrustum*>	m_arrShadowFrustum;
	};

	class RENDER_API PointLight : public Light
	{
		DECL_OBJECT(PointLight)

	public:
		PointLight(GameObject* pGameObj):Light(pGameObj) {m_eLightType = LIGHT_POINT;}
		
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
		enum {NUM_PSSM = 4};

	public:
		DirectonalLight(GameObject* pGameObj);

		Vector3			GetDirection();

		virtual void	SetCreateShadow(bool bCreateShaow);

		virtual void	UpdateShadowFrustum(Camera* pCamera);

		float*			GetSplitPos();

	private:
		Matrix4x4		CalculateCropMatrix(const AABB& cropAABB);

	private:
		float			m_fSplitPos[2][NUM_PSSM + 1];

		float			m_fShadowOffset;
		
	};


	class RENDER_API SpotLight : public Light
	{
		DECL_OBJECT(SpotLight)

	public:
		SpotLight(GameObject* pGameObj):Light(pGameObj) {m_eLightType = LIGHT_SPOT;}

		NodeTransform	GetTransform() {return m_tsfWS;}

		void			SetTransform(const NodeTransform& tsfWS) {m_tsfWS = tsfWS;}

		void			LookAt(const Vector3& vEye,const Vector3& vAt,const Vector3& vUp);	

		Matrix4x4		GetViewMatrix() {return m_mView;}

		Matrix4x4		GetProjmatrix()	{return m_mProj;}

	private:
		NodeTransform	m_tsfWS;

		float			m_fAspectRatio;
		float			m_fNearClip;
		float			m_fFarClip;
		float			m_fFOV;

		Vector3			m_vTarget;
		Vector3			m_vUpVector;
		Matrix4x4		m_mView;
		Matrix4x4		m_mProj;
	};


	DeclareRefPtr(PointLight);
	DeclareRefPtr(DirectonalLight);

}

#endif


