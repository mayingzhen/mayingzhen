#ifndef  _Frustum__H__
#define  _Frustum__H__

namespace ma
{
	class COMMON_API Frustum  
	{
//	public:
// 		enum Visble
// 		{
// 			NONE,
// 			FULL,
// 			PARTIAL,
// 		};

	public:
		Frustum();

		Frustum(const Matrix4x4& mViewProj); 

		//Visble	VisibleTest(const AABB& box) const;

		//bool	IsPointIn(const Vector3& point) const;

	public:
		Vector3	vPts[8];
		//PLANE	vPlanes[6];
		//int			nVertexLUT[6];

		Matrix4x4	m_matViewProj;
	};
}

#endif
