#ifndef  _Camera__H__
#define  _Camera__H__

namespace ma
{
	class Camera : public Component
	{
		DECL_OBJECT(Camera)

	public:

		Camera();
	
		~Camera();

		static void			RegisterAttribute();

		void				SetPerspective(float fFOV,float fAspect,float fNear,float fFar);

		const Matrix4&		GetMatView();

		const Matrix4&		GetMatProj();

		const Matrix4&		GetMatViewProj();
		
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

		virtual void		MarkDirty();

	private:
		MatViewProj			m_matViewProj;

		float				m_fAspect;
		float				m_fFOV;	
		float				m_fNear;
		float				m_fFar;
		uint32				m_nWidth;
		uint32				m_nHeight;

		float				m_fNearMin;		

		Frustum				m_frustum;
	};

}

#endif