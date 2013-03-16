#include "Frustum.h"

namespace ma
{
	Frustum::Frustum()
	{

	}

	Frustum::Frustum(const D3DXMATRIX &mViewProj)
	{
		m_matViewProj = mViewProj;

		float nearZ = 0.0f;
		float farZ = 1.0f; // gl -1 ~ 1
		D3DXVECTOR3 _near_left_top(-1,1,nearZ),_near_left_top_world;
		D3DXVECTOR3 _near_left_bottom(-1,-1,nearZ),_near_left_bottom_world;
		D3DXVECTOR3 _near_right_top(1,1,nearZ),_near_right_top_world;
		D3DXVECTOR3 _near_right_bottom(1,-1,nearZ),_near_right_bottom_world;

		D3DXVECTOR3 _far_left_top(-1,1,farZ),_far_left_top_world;
		D3DXVECTOR3 _far_left_bottom(-1,-1,farZ),_far_left_bottom_world;
		D3DXVECTOR3 _far_right_top(1,1,farZ),_far_right_top_world;
		D3DXVECTOR3 _far_right_bottom(1,-1,farZ),_far_right_bottom_world;


		D3DXMATRIX __invProjViewMatrix;
		D3DXMatrixInverse(&__invProjViewMatrix,0,&mViewProj);

		D3DXVec3TransformCoord(&_near_left_top_world,&_near_left_top,&__invProjViewMatrix);
		D3DXVec3TransformCoord(&_near_left_bottom_world,&_near_left_bottom,&__invProjViewMatrix);
		D3DXVec3TransformCoord(&_near_right_top_world,&_near_right_top,&__invProjViewMatrix);
		D3DXVec3TransformCoord(&_near_right_bottom_world,&_near_right_bottom,&__invProjViewMatrix);

		D3DXVec3TransformCoord(&_far_left_top_world,&_far_left_top,&__invProjViewMatrix);
		D3DXVec3TransformCoord(&_far_left_bottom_world,&_far_left_bottom,&__invProjViewMatrix);
		D3DXVec3TransformCoord(&_far_right_top_world,&_far_right_top,&__invProjViewMatrix);
		D3DXVec3TransformCoord(&_far_right_bottom_world,&_far_right_bottom,&__invProjViewMatrix);

		vPts[0] = _near_left_top_world;
		vPts[1] = _near_left_bottom_world;
		vPts[2] = _near_right_top_world;
		vPts[3] = _near_right_bottom_world;
		vPts[4] = _far_left_top_world;
		vPts[5] = _far_left_bottom_world;
		vPts[6] = _far_right_top_world;
		vPts[7] = _far_right_bottom_world;
	}
}
