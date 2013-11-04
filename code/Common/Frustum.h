#ifndef  _Frustum__H__
#define  _Frustum__H__

namespace ma
{
	class COMMON_API Frustum  
	{
	public:
		Frustum(const Matrix4x4& mViewProj); 

	public:
		Vector3		m_pPoints[8];
	};
}

#endif
