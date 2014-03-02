#ifndef  _Camera__H__
#define  _Camera__H__

namespace ma
{
	class RENDER_API Camera : public Component
	{
		DECL_OBJECT(Camera)

	public:

		Camera(GameObject* pGameObj);
	
		~Camera();

		virtual	void		UpdateTransform();

		void				SetPerspective(float fFOV,float fAspect,float fNear,float fFar);

		MatViewProj&		GetMatViewProj() {return m_matViewProj;}
		
		float				GetNearClip() {return m_fNear;}

		float				GetFarClip() {return m_fFar;}

		float				GetFov() {return m_fFOV;}

		float				GetAspect() {return m_fAspect;}

		void				FitAABB(const AABB& aabb, float fMargin);

		Vector3				ProjToWorldNormal(const Vector2* pVec,float fDepth);

		bool				IsCull(AABB aabb);

		void				AdjustPlanes(const AABB& aabbWorld);

		void				GetWorldRayCast(const Vector2& clientSize,const Vector2& mousePos, Vector3& worldOrig, Vector3& worldDir);

	protected:

		Vector2				GetNearPlaneSize() const;

	private:
		MatViewProj			m_matViewProj;

		float				m_fAspect;
		float				m_fFOV;	
		float				m_fNear;
		float				m_fFar;

		float				m_fNearMin;		
	};
}

#endif