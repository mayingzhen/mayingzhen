#ifndef  _Light__H__
#define  _Light__H__



namespace ma
{
	enum LightType
	{
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,			
		LIGHT_POINT,	
	};


	class ENGINE_API Light 
	{
	public:
		Light();

		LightType		GetLightType() {return m_eLightType;}

		const Vector4&	GetLightColor() {return m_cLightColor;}

		void			SetLightColor(const Vector4& cLightColor) {m_cLightColor = cLightColor;}

		bool			IsCreateShadow() {return m_bCreateShadow;}

		void			SetCreateShadow(bool bCreateShaow) {m_bCreateShadow = bCreateShaow;}	

	protected:
		LightType		m_eLightType;

		Vector4			m_cLightColor;

		bool			m_bCreateShadow;
	};

	class ENGINE_API PointLight : public Light
	{
	public:
		PointLight():Light() {m_eLightType = LIGHT_POINT;}
		
		Vector3 GetPos() {return m_vPos;}

		void	SetPos(const Vector3& vPos) {m_vPos = vPos;}

		float	GetRadius() {return m_fRadius;}

		void	SetRadius(float fRadius) {m_fRadius = fRadius ? fRadius : 1.0f;}

	private:
		Vector3		m_vPos;

		float		m_fRadius;
	};


	class ENGINE_API DirectonalLight : public Light
	{
	public:
		DirectonalLight():Light() {m_eLightType = LIGHT_DIRECTIONAL;}

		const Vector3&	GetDirection() {return m_vDirection;}

		void			SetDirection(const Vector3& vDirection) {Vec3Normalize(&m_vDirection,&vDirection);}

	private:
		Vector3			m_vDirection;
	};


	class ENGINE_API SpotLight : public Light
	{
	public:
		SpotLight():Light() {m_eLightType = LIGHT_SPOT;}

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

}

#endif


