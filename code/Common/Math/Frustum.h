#ifndef  _Frustum__H__
#define  _Frustum__H__

namespace ma
{
	class Frustum
	{
	public:
		Frustum(void);
		Frustum(const Frustum& rkFrustum);

		~Frustum(void);

		void Update(const Matrix4& matViewProj,bool bGLSystem);

		enum Visibility
		{
			Visibility_NONE,
			Visibility_PARTITAL,
			Visibility_FULL,
		};
		Visibility Intersect(const AABB& box) const;

		Visibility IntersectSIMD(const Vector3& vCenter,const Vector3& vHalfSiz) const;

		/** Tests whether the given container is visible in the Frustum.
			@param
				bound Bounding box to be checked (world space)
			@returns
				If the box was visible, true is returned.
			@par
				Otherwise, false is returned.
		*/
		bool isVisible(const AABB& bound) const;

		/** Tests whether the given container is visible in the Frustum.
			@param
				bound Bounding sphere to be checked (world space)
			@returns
				If the sphere was visible, true is returned.
			@par
				Otherwise, false is returned.
		*/
		bool isVisible(const Sphere& bound) const;

		/** Tests whether the given vertex is visible in the Frustum.
			@param
				vert Vertex to be checked (world space)
			@returns
				If the box was visible, true is returned.
			@par
				Otherwise, false is returned.
		*/
		bool isVisible(const Vector3& vert) const;

		const AABB&	GetAABB() const {return m_aabb;}

		enum Points
		{ 
			near_right_top, 
			near_left_top,  
			near_left_bottom, 
			near_right_bottom, 
			far_right_top, 
			far_left_top, 
			far_left_bottom,
			far_right_bottom,
			PointsNumber
		};
		const Vector3& GetPoint(Points ePoint) const {return m_pPoints[ePoint];} 

		const Vector3* GetPoint() const {return m_pPoints;}

		void Transformed(Matrix4 matTrans);

	private:
		void UpdatePoint(const Matrix4 &invProjViewMatrix,bool bGLSystem);

		void UpdatePlanes();
	
		void UpdateSIMDPlanes();

	private:
		enum FrustumPlane
		{
			FRUSTUM_PLANE_NEAR   = 0,
			FRUSTUM_PLANE_FAR    = 1,
			FRUSTUM_PLANE_LEFT   = 2,
			FRUSTUM_PLANE_RIGHT  = 3,
			FRUSTUM_PLANE_TOP    = 4,
			FRUSTUM_PLANE_BOTTOM = 5,
			FRUSTUM_PLANE_NUM = 6
		};
		ma::Plane m_rgPlane[FRUSTUM_PLANE_NUM];

		Vector3	m_pPoints[PointsNumber];

		AABB	m_aabb;

		struct SIMDPlane
		{
			float x,y,z,w;

			SIMDPlane() {x = y = z = w = 0;}

			SIMDPlane(float fx, float fy, float fz, float fw) {x = fx,y = fy,z = fz, w = fw;}
		};

		/** This is the set of planes pre-permuted to SSE/Altivec form */
		SIMDPlane* m_rgSIMDPlane;
	
	};
}

#endif
