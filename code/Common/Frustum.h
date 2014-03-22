#ifndef  _Frustum__H__
#define  _Frustum__H__

namespace ma
{
	
	class COMMON_API Frustum  
	{

	public:
		enum Visibility
		{
			Visibility_NONE,
			Visibility_PARTITAL,
			Visibility_FULL,
		};

		enum Points
		{
			near_left_top,
			near_left_bottom,
			near_right_top,
			near_right_bottom,
			far_left_top,
			far_left_bottom,
			far_right_top,
			far_right_bottom,
			PointsNumber
		};

		enum Plans
		{
			near_Plane,
			far_Plane,
			Left_Plane,
			Right_Plane,
			top_Plane,
			bottom_Plane,
			PlansNumber
		};



	public:
		Frustum();

		void		Update(const Matrix4x4& mViewProj,float nearZ, float farZ);

		Visibility	Intersect(const AABB& box) const;

		bool		isVisible(const AABB& bound) const;

	public:
		Vector3		m_pPoints[PointsNumber];

		Plane		m_pPlane[PlansNumber];
	};
}

#endif
