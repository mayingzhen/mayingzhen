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

		//virtual	void	Update();

		LightType		GetLightType() {return m_eLightType;}

		void			SetLigtType(LightType eType) {m_eLightType = eType;}

		Matrix4x4		GetViewMatrix() {return m_mView;}

		Matrix4x4		GetProjmatrix()	{return m_mProj;}

		Vector3		GetDiffuse() {return m_vLightDiffuse;}

		float			GetRadius() {return m_fRadius;}

		void			SetRadius(float fRadius) {m_fRadius = fRadius;}

		Vector3		GetDirection() {return m_vDirection;}

		bool			IsCreateShadow() {return m_bCreateShadow;}

		void			SetCreateShadow(bool bCreateShaow) {m_bCreateShadow = bCreateShaow;}

		void			LookAt(const Vector3* pEye,const Vector3* pAt = NULL,const Vector3* pUp = NULL);

	protected:
// 		void			SyncFromSceneNode();
// 
// 		void			SyncToSceneNode();

	protected:
		LightType		m_eLightType;

		Vector3		m_vLightDiffuse;

		Vector3		m_vTarget;
		Vector3		m_vUpVector;
		Matrix4x4		m_mView;
		Matrix4x4		m_mProj;
		

		// Point Light
		float			m_fRadius;

		// directional Ligt
		Vector3		m_vDirection;
		bool			m_bCreateShadow;

		// spot Light
		float			m_fAspectRatio;
		float			m_fNearClip;
		float			m_fFarClip;
		float			m_fFOV;
	};

}

#endif


