#pragma once

namespace ma
{
	class Camera : public SceneNode
	{	

	public:

		Camera();
	
		~Camera();

		DECL_OBJECT(Camera)

		static void			RegisterAttribute();

		virtual void		Update();

		void				SetPerspective(float fFOV,float fAspect,float fNear,float fFar);

		const Matrix4&		GetMatView();
		const Matrix4&		GetMatProj();
		const Matrix4&		GetMatViewProj();
		const Matrix4&		GetMatViewProjInv();
		
		float				GetNearClip() const {return m_fNear;}
		void				SetNearClip(float fNear);

		float				GetFarClip() const {return m_fFar;}
		void				SetFarClip(float fFar);
		
		float				GetFov() const {return m_fFOV;}
		float				GetFovDegrees() const {return RadiansToDegrees(m_fFOV);}
		void				SetFovDegrees(float fFovDegrees);

		float				GetAspect() const {return m_fAspect;}
		void				SetAspect(float fAsPect);

		void				FitAABB(const AABB& aabb, float fMargin);

		void				AdjustPlanes(const AABB& aabbWorld);

		Ray					GetWorldRayCast(const Vector2& mousePos);

		const Frustum&		GetFrustum() const {return m_frustum;}

	protected:

		Vector2				GetNearPlaneSize() const;

		void				UpdateViewProjMatrix();

	private:
		Matrix4				m_matView, m_matViewInv;
		Matrix4				m_matProj, m_matProjInv;
		Matrix4				m_matVP, m_matVPInv;
		float				m_fAspect;
		float				m_fFOV;	
		float				m_fNear;
		float				m_fFar;
		uint32_t			m_nWidth;
		uint32_t			m_nHeight;

		float				m_fNearMin;		

		Frustum				m_frustum;
 	};
	
	RefPtr<Camera> CreateCamera();
}
