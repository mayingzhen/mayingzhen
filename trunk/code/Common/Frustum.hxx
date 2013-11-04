#include "Frustum.h"

namespace ma
{
	Frustum::Frustum(const Matrix4x4 &mViewProj)
	{
		float nearZ = 0.0f;
		float farZ = 1.0f; // gl -1 ~ 1
		Vector3 _near_left_top(-1,1,nearZ),_near_left_top_world;
		Vector3 _near_left_bottom(-1,-1,nearZ),_near_left_bottom_world;
		Vector3 _near_right_top(1,1,nearZ),_near_right_top_world;
		Vector3 _near_right_bottom(1,-1,nearZ),_near_right_bottom_world;

		Vector3 _far_left_top(-1,1,farZ),_far_left_top_world;
		Vector3 _far_left_bottom(-1,-1,farZ),_far_left_bottom_world;
		Vector3 _far_right_top(1,1,farZ),_far_right_top_world;
		Vector3 _far_right_bottom(1,-1,farZ),_far_right_bottom_world;


		Matrix4x4 invProjViewMatrix;
		MatrixInverse(&invProjViewMatrix,0,&mViewProj);

		Vec3TransformCoord(&_near_left_top_world,&_near_left_top,&invProjViewMatrix);
		Vec3TransformCoord(&_near_left_bottom_world,&_near_left_bottom,&invProjViewMatrix);
		Vec3TransformCoord(&_near_right_top_world,&_near_right_top,&invProjViewMatrix);
		Vec3TransformCoord(&_near_right_bottom_world,&_near_right_bottom,&invProjViewMatrix);

		Vec3TransformCoord(&_far_left_top_world,&_far_left_top,&invProjViewMatrix);
		Vec3TransformCoord(&_far_left_bottom_world,&_far_left_bottom,&invProjViewMatrix);
		Vec3TransformCoord(&_far_right_top_world,&_far_right_top,&invProjViewMatrix);
		Vec3TransformCoord(&_far_right_bottom_world,&_far_right_bottom,&invProjViewMatrix);

		m_pPoints[0] = _near_left_top_world;
		m_pPoints[1] = _near_left_bottom_world;
		m_pPoints[2] = _near_right_top_world;
		m_pPoints[3] = _near_right_bottom_world;
		m_pPoints[4] = _far_left_top_world;
		m_pPoints[5] = _far_left_bottom_world;
		m_pPoints[6] = _far_right_top_world;
		m_pPoints[7] = _far_right_bottom_world;
	}
}
