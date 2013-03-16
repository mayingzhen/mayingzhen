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

		Frustum(const D3DXMATRIX& mViewProj); 

		//Visble	VisibleTest(const AABB& box) const;

		//bool	IsPointIn(const D3DXVECTOR3& point) const;

	public:
		D3DXVECTOR3	vPts[8];
		//D3DXPLANE	vPlanes[6];
		//int			nVertexLUT[6];

		D3DXMATRIX	m_matViewProj;
	};
}

#endif
