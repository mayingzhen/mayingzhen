#ifndef  _Frustum__H__
#define  _Frustum__H__

namespace ma
{
	class COMMON_API Frustum  
	{
	public:
		Frustum();

		Frustum(const Matrix4x4& mViewProj); 

	public:
		Vector3	vPts[8];

		Matrix4x4	m_matViewProj;
	};
}

#endif
