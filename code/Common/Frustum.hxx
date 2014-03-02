#include "Frustum.h"

namespace ma
{
	Frustum::Frustum()
	{

	}

	void Frustum::Update(const Matrix4x4 &mViewProj)
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

	Frustum::Visibility Frustum::Intersect(const AABB& box) const
	{
		return Visibility_FULL;
// 		if (box.isNull())
// 		{
// 			return Visibility_NONE;
// 		}
// 
// 		Vector3 vCenter = box.getCenter();
// 		Vector3 vHalfSize = box.getHalfSize();
// 
// 		bool bAllInSide = true;
// 		for (int i = 0;i< 6;++i)
// 		{
// 			const Plane& plane = m_rgPlane[i];
// 			Plane::Side side = plane.getSide(vCenter, vHalfSize);
// 			if (side == Plane::NEGATIVE_SIDE)
// 			{
// 				return Visibility_NONE;
// 			}
// 
// 			if (side == Plane::BOTH_SIDE)
// 			{
// 				bAllInSide = false;
// 			}
// 		}
// 
// 		if (bAllInSide)
// 		{
// 			return Visibility_FULL;
// 		}
// 		else
// 		{
// 			return Visibility_PARTITAL;
// 		}
	}

	bool Frustum::isVisible(const AABB& bound) const
	{
// 		// Null boxes always invisible
// 		if (bound.isNull()) return false;
// 
// 		// Infinite boxes always visible
// 		if (bound.isInfinite()) return true;
// 
// 		// Get centre of the box
// 		Vector3 centre = bound.getCenter();
// 		// Get the half-size of the box
// 		Vector3 halfSize = bound.getHalfSize();
// 
// 		// For each plane, see if all points are on the negative side
// 		// If so, object is not visible
// 		for (int plane = 0; plane < 6; ++plane)
// 		{
// 			Plane::Side side = m_rgPlane[plane].getSide(centre, halfSize);
// 			if (side == Plane::NEGATIVE_SIDE)
// 			{
// 				return false;
// 			}
// 
// 		}

		return true;
	}
}
