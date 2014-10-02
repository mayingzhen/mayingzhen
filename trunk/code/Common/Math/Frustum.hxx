#include "Frustum.h"

namespace ma
{
	Frustum::Frustum(void)
	{
	}

	Frustum::~Frustum(void)
	{
	}

	void Frustum::UpdatePoint(const Matrix4 &invProjViewMatrix,bool bGLSystem)
	{
		float nearZ = 0.0f;
		float farZ = 1.0f; // gl -1 ~ 1
		if (bGLSystem) 
		{
			nearZ = -1.0f;
			farZ = 1.0f;
		}

		m_pPoints[near_left_top] = invProjViewMatrix * Vector3(-1,1,nearZ);
		m_pPoints[near_left_bottom] = invProjViewMatrix * Vector3(-1,-1,nearZ);
		m_pPoints[near_right_top] = invProjViewMatrix * Vector3(1,1,nearZ);
		m_pPoints[near_right_bottom] = invProjViewMatrix * Vector3(1,-1,nearZ);

		m_pPoints[far_left_top] = invProjViewMatrix * Vector3(-1,1,farZ);
		m_pPoints[far_left_bottom] = invProjViewMatrix * Vector3(-1,-1,farZ);
		m_pPoints[far_right_top] = invProjViewMatrix * Vector3(1,1,farZ);
		m_pPoints[far_right_bottom] = invProjViewMatrix * Vector3(1,-1,farZ);

		m_aabb.setNull();
		for (int i = 0; i < PointsNumber; ++i)
		{
			m_aabb.merge(m_pPoints[i]);
		}
	}

	void Frustum::UpdatePlanes()
	{
		m_rgPlane[FRUSTUM_PLANE_NEAR].redefine(m_pPoints[near_left_top], m_pPoints[near_right_top], m_pPoints[near_right_bottom]);
		m_rgPlane[FRUSTUM_PLANE_FAR].redefine(m_pPoints[far_left_top], m_pPoints[far_right_bottom], m_pPoints[far_right_top]);
		m_rgPlane[FRUSTUM_PLANE_LEFT].redefine(m_pPoints[near_left_top], m_pPoints[near_left_bottom], m_pPoints[far_left_top]);
		m_rgPlane[FRUSTUM_PLANE_RIGHT].redefine(m_pPoints[near_right_top], m_pPoints[far_right_top], m_pPoints[near_right_bottom]);
		m_rgPlane[FRUSTUM_PLANE_TOP].redefine(m_pPoints[near_left_top], m_pPoints[far_left_top], m_pPoints[near_right_top]);
		m_rgPlane[FRUSTUM_PLANE_BOTTOM].redefine(m_pPoints[near_left_bottom], m_pPoints[near_right_bottom], m_pPoints[far_left_bottom]);
	}


	void Frustum::Update(const Matrix4& matViewProj,bool bGLSystem)
	{
		UpdatePoint(matViewProj.inverse(),bGLSystem);

		UpdatePlanes();
	}

	void Frustum::Transformed(Matrix4 matTrans)
	{
		for (unsigned i = 0; i < PointsNumber; ++i)
		{
			m_pPoints[i] = matTrans * m_pPoints[i];
		}

		m_aabb.setNull();
		for (int i = 0; i < PointsNumber; ++i)
		{
			m_aabb.merge(m_pPoints[i]);
		}

		UpdatePlanes();
	}

	Frustum::Visibility Frustum::Intersect( const AABB& box ) const
	{
		if (box.isNull())
		{
			return Visibility_NONE;
		}

		Vector3 vCenter = box.getCenter();
		Vector3 vHalfSize = box.getHalfSize();

		bool bAllInSide = true;
		for (int i = 0;i< 6;++i)
		{
			const Plane& plane = m_rgPlane[i];
			Plane::Side side = plane.getSide(vCenter, vHalfSize);
			if (side == Plane::NEGATIVE_SIDE)
			{
				return Visibility_NONE;
			}

			if (side == Plane::BOTH_SIDE)
			{
				bAllInSide = false;
			}
		}

		if (bAllInSide)
		{
			return Visibility_FULL;
		}
		else
		{
			return Visibility_PARTITAL;
		}
	}


	//-----------------------------------------------------------------------
	bool Frustum::isVisible(const AABB& bound) const
	{
		// Null boxes always invisible
		if (bound.isNull()) return false;

		// Infinite boxes always visible
		if (bound.isInfinite()) return true;

		// Get centre of the box
		Vector3 centre = bound.getCenter();
		// Get the half-size of the box
		Vector3 halfSize = bound.getHalfSize();

		// For each plane, see if all points are on the negative side
		// If so, object is not visible
		for (int plane = 0; plane < 6; ++plane)
		{
			Plane::Side side = m_rgPlane[plane].getSide(centre, halfSize);
			if (side == Plane::NEGATIVE_SIDE)
			{
				return false;
			}

		}

		return true;
	}

	//-----------------------------------------------------------------------
	bool Frustum::isVisible(const Vector3& vert) const
	{
		// For each plane, see if all points are on the negative side
		// If so, object is not visible
		for (int plane = 0; plane < 6; ++plane)
		{
			if (m_rgPlane[plane].getSide(vert) == Plane::NEGATIVE_SIDE)
			{
				return false;
			}

		}

		return true;
	}
	//-----------------------------------------------------------------------
	bool Frustum::isVisible(const Sphere& sphere) const
	{
		// For each plane, see if sphere is on negative side
		// If so, object is not visible
		for (int plane = 0; plane < 6; ++plane)
		{
			// If the distance from sphere center to plane is negative, and 'more negative' 
			// than the radius of the sphere, sphere is outside frustum
			if (m_rgPlane[plane].getDistance(sphere.getCenter()) < -sphere.getRadius())
			{
				return false;
			}

		}

		return true;
	}

}
