#include "Frustum.h"
#include "Math.h"
#include "MathSSE.h"


namespace ma
{
	Frustum::Frustum(void)
	{
		m_rgSIMDPlane = NULL;
	}

	Frustum::Frustum(const Frustum& rkFrustum)
	{
		memcpy(m_rgPlane,rkFrustum.m_rgPlane,sizeof(m_rgPlane));
		memcpy(m_pPoints,rkFrustum.m_pPoints,sizeof(m_pPoints));
		m_aabb = rkFrustum.m_aabb;
		m_rgSIMDPlane = NULL;
	}

	Frustum::~Frustum(void)
	{
		AlignedMemory_deallocate(m_rgSIMDPlane);
	}

	void Frustum::UpdatePoint(const Matrix4 &invProjViewMatrix,bool bGLSystem,bool bInvertedY)
	{
		float nearZ = 0.0f;
		float farZ = 1.0f; // gl -1 ~ 1
		if (bGLSystem) 
		{
			nearZ = -1.0f;
			farZ = 1.0f;
		}

		Vector3	pPoints[PointsNumber];
		pPoints[near_left_top] = Vector3(-1, 1, nearZ);
		pPoints[near_left_bottom] = Vector3(-1, -1, nearZ);
		pPoints[near_right_top] = Vector3(1, 1 , nearZ);
		pPoints[near_right_bottom] =  Vector3(1, -1, nearZ);

		pPoints[far_left_top] =  Vector3(-1, 1, farZ);
		pPoints[far_left_bottom] =  Vector3(-1, -1, farZ);
		pPoints[far_right_top] =  Vector3(1, 1, farZ);
		pPoints[far_right_bottom] = Vector3(1, -1, farZ);

		if (bInvertedY)
		{
			for (uint32_t i = 0; i < PointsNumber; ++i)
			{
				pPoints[i].y *= -1.0f;
			}
		}

		for (uint32_t i = 0; i < PointsNumber; ++i)
		{
			m_pPoints[i] = invProjViewMatrix * pPoints[i];
		}

		m_aabb.setNull();
		for (int i = 0; i < PointsNumber; ++i)
		{
			m_aabb.merge(m_pPoints[i]);
		}
	}

	void Frustum::Update(const Matrix4& matViewProj, bool bGLSystem, bool bInvertedY)
	{
		UpdatePoint(matViewProj.inverse(),bGLSystem,bInvertedY);

		UpdatePlanes();

		UpdateSIMDPlanes();
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
	
	void Frustum::UpdatePlanes()
	{
		m_rgPlane[FRUSTUM_PLANE_NEAR].redefine(m_pPoints[near_left_top], m_pPoints[near_right_top], m_pPoints[near_right_bottom]);
		m_rgPlane[FRUSTUM_PLANE_FAR].redefine(m_pPoints[far_left_top], m_pPoints[far_right_bottom], m_pPoints[far_right_top]);
		m_rgPlane[FRUSTUM_PLANE_LEFT].redefine(m_pPoints[near_left_top], m_pPoints[near_left_bottom], m_pPoints[far_left_top]);
		m_rgPlane[FRUSTUM_PLANE_RIGHT].redefine(m_pPoints[near_right_top], m_pPoints[far_right_top], m_pPoints[near_right_bottom]);
		m_rgPlane[FRUSTUM_PLANE_TOP].redefine(m_pPoints[near_left_top], m_pPoints[far_left_top], m_pPoints[near_right_top]);
		m_rgPlane[FRUSTUM_PLANE_BOTTOM].redefine(m_pPoints[near_left_bottom], m_pPoints[near_right_bottom], m_pPoints[far_left_bottom]);
	}

	void Frustum::UpdateSIMDPlanes()
	{
		if (m_rgSIMDPlane == NULL)
		{
			m_rgSIMDPlane = static_cast<SIMDPlane*>( AlignedMemory_allocate( 8 * sizeof(SIMDPlane) , 16 ) );
			for (uint32_t i = 0; i < 8; ++i)
			{
				m_rgSIMDPlane[i] = SIMDPlane();
			}
		}

		m_rgSIMDPlane[0] = SIMDPlane(m_rgPlane[0].normal.x,m_rgPlane[1].normal.x,m_rgPlane[2].normal.x,m_rgPlane[3].normal.x);
		m_rgSIMDPlane[1] = SIMDPlane(m_rgPlane[0].normal.y,m_rgPlane[1].normal.y,m_rgPlane[2].normal.y,m_rgPlane[3].normal.y);
		m_rgSIMDPlane[2] = SIMDPlane(m_rgPlane[0].normal.z,m_rgPlane[1].normal.z,m_rgPlane[2].normal.z,m_rgPlane[3].normal.z);
		m_rgSIMDPlane[3] = SIMDPlane(m_rgPlane[0].d,		 m_rgPlane[1].d,	   m_rgPlane[2].d,		 m_rgPlane[3].d);

		m_rgSIMDPlane[4] = SIMDPlane(m_rgPlane[4].normal.x,m_rgPlane[5].normal.x,m_rgPlane[4].normal.x,m_rgPlane[4].normal.x);
		m_rgSIMDPlane[5] = SIMDPlane(m_rgPlane[4].normal.y,m_rgPlane[5].normal.y,m_rgPlane[4].normal.y,m_rgPlane[4].normal.y);
		m_rgSIMDPlane[6] = SIMDPlane(m_rgPlane[4].normal.z,m_rgPlane[5].normal.z,m_rgPlane[4].normal.z,m_rgPlane[4].normal.z);
		m_rgSIMDPlane[7] = SIMDPlane(m_rgPlane[4].d,		 m_rgPlane[5].d,	   m_rgPlane[4].d,       m_rgPlane[4].d);
	}

	Frustum::Visibility Frustum::Intersect( const AABB& box ) const
	{
		if (box.isNull())
		{
			return Visibility_NONE;
		}

		if (box.isInfinite())
		{
			return Visibility_PARTITAL;
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

	static  bool _isAlignedForSSE(const void *p)
	{
		return (((size_t)p) & 15) == 0;
	}

	Frustum::Visibility Frustum::IntersectSIMD(const Vector3& vCenter,const Vector3& vHalfSize) const
	{
		ASSERT(m_rgSIMDPlane);

		const VectorRegister BoxOrigin = VectorLoadFloat3( &vCenter );
		const VectorRegister BoxExtent = VectorLoadFloat3( &vHalfSize );

		//checkSlow(PermutedPlanes.Num() % 4 == 0);

		// Splat origin into 3 vectors
		VectorRegister OrigX = VectorReplicate(BoxOrigin, 0);
		VectorRegister OrigY = VectorReplicate(BoxOrigin, 1);
		VectorRegister OrigZ = VectorReplicate(BoxOrigin, 2);

		// Splat the abs for the pushout calculation
		VectorRegister AbsExt = VectorAbs(BoxExtent);
		VectorRegister AbsExtentX = VectorReplicate(AbsExt, 0);
		VectorRegister AbsExtentY = VectorReplicate(AbsExt, 1);
		VectorRegister AbsExtentZ = VectorReplicate(AbsExt, 2);
		// Since we are moving straight through get a pointer to the data
		const SIMDPlane* /*RESTRICT*/ PermutedPlanePtr = m_rgSIMDPlane;
		// Process four planes at a time until we have < 4 left
		for (int Count = 0; Count < 8; Count += 4 )
		{
			// Load 4 planes that are already all Xs, Ys, ...
			ASSERT(_isAlignedForSSE(PermutedPlanePtr));
			VectorRegister PlanesX = VectorLoadAligned(PermutedPlanePtr);
			PermutedPlanePtr++;
			VectorRegister PlanesY = VectorLoadAligned(PermutedPlanePtr);
			PermutedPlanePtr++;
			VectorRegister PlanesZ = VectorLoadAligned(PermutedPlanePtr);
			PermutedPlanePtr++;
			VectorRegister PlanesW = VectorLoadAligned(PermutedPlanePtr);
			PermutedPlanePtr++;
			// Calculate the distance (x * x) + (y * y) + (z * z) + w
			VectorRegister DistX = VectorMultiply(OrigX,PlanesX);
			VectorRegister DistY = VectorMultiplyAdd(OrigY,PlanesY,DistX);
			VectorRegister DistZ = VectorMultiplyAdd(OrigZ,PlanesZ,DistY);
			VectorRegister Distance = VectorAdd(DistZ,PlanesW);
			// Now do the push out FMath::Abs(x * x) + FMath::Abs(y * y) + FMath::Abs(z * z)
			VectorRegister PushX = VectorMultiply(AbsExtentX,VectorAbs(PlanesX));
			VectorRegister PushY = VectorMultiplyAdd(AbsExtentY,VectorAbs(PlanesY),PushX);
			VectorRegister PushOut = VectorMultiplyAdd(AbsExtentZ,VectorAbs(PlanesZ),PushY);
			VectorRegister NegPushOut = VectorMultiply(GlobalVectorConstants::FloatMinusOne,PushOut);

			// Check for completely outside
			int temp = VectorAnyGreaterThan(NegPushOut,Distance);
			if (temp)
			{
				return Visibility_NONE;
			}
		}

		return Visibility_FULL;
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
