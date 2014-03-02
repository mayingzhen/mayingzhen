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


	public:
		Frustum();

		void Update(const Matrix4x4& mViewProj);

		Visibility Intersect(const AABB& box) const;

		bool isVisible(const AABB& bound) const;

	public:
		Vector3		m_pPoints[8];
	};
}

#endif
