#ifndef  _CAMERA__H__
#define  _CAMERA__H__



namespace ma
{
	class ENGINE_API Camera 
	{
	public:
		enum {NUM_PSSM = 4};

		Camera();
		
		~Camera();			

		void				LookAt(const Vector3& vEye,const Vector3& vAt,const Vector3& vUp);

		void				SetPerspective(float fFOV,float fAspect,float fNear,float fFar);

		NodeTransform		GetTransform() const;

		void				SetTransform(const NodeTransform& tsfCamra);

		const Matrix4x4&	GetViewMatrix() const {return m_matView;}

		const Matrix4x4&	GetProjMatrix() const {return m_matProj;}
		
		Matrix4x4			GetViewProjMatrix() const {return m_matView * m_matProj;}
		
		float				GetNearClip() {return m_fNear;}

		float				GetFarClip() {return m_fFar;}
		
		Frustum				GetSplitFrustum(int index) {return m_arrSplitFrustum[index];}

		float*				GetSplitPos() {return m_fSplitPos;}

		void				FitAABB(const AABB& aabb, float fMargin);

		Vector3				ProjToWorldNormal(const Vector2* pVec) const;

		Vector3				GetLookAtPos() {return m_vLookAtPt;}

	protected:
		void				CalculateSplitPositions();

		void				CalculateFrustum();

		Frustum				CalculateFrustum(float fNear, float fFar);

		Vector2				GetNearPlaneSize() const;

	private:
		Matrix4x4		m_matView;
		Matrix4x4		m_matProj;

		float			m_fAspect;
		float			m_fFOV;	
		float			m_fNear;
		float			m_fFar;

		Vector3			m_vLookAtPt;
		Vector3			m_vEyePt;
		Vector3			m_vUpVector;

		//bool			m_bProjDirty;

		//bool			m_bOrtho;
		float			m_fHeight;

		float m_fSplitPos[NUM_PSSM + 1];

		Frustum m_arrSplitFrustum[NUM_PSSM];

		Frustum	m_frustum;
	};
}

#endif