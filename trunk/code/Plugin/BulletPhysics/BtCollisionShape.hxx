#include "BulletPhysics/BtCollisionShape.h"

namespace ma
{	
	BulletBoxCollisionShape::BulletBoxCollisionShape()
	{
		TransformSetIdentity(&m_tsfLS);
		m_vSize = Vector3(0,0,0);
	}

	BulletBoxCollisionShape::~BulletBoxCollisionShape()
	{

	}

	void BulletBoxCollisionShape::SetSize(const Vector3& vSize)
	{
		m_vSize = vSize;
	}

	Vector3 BulletBoxCollisionShape::GetSize()
	{
		return m_vSize;
	}


	BulletSphereCollisionShape::BulletSphereCollisionShape()
	{
		TransformSetIdentity(&m_tsfLS);
		m_fRadius = 0;
	}

	BulletSphereCollisionShape::~BulletSphereCollisionShape()
	{

	}

	void BulletSphereCollisionShape::SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}

	float BulletSphereCollisionShape::GetRadius()
	{
		return m_fRadius;
	}


	BulletCapsuleCollisionShape::BulletCapsuleCollisionShape()
	{
		TransformSetIdentity(&m_tsfLS);
		m_fRadius = 0;
		m_fHeight = 0;
	}

	BulletCapsuleCollisionShape::~BulletCapsuleCollisionShape()
	{

	}

	void BulletCapsuleCollisionShape::SetHeight(float fHeight)
	{
		m_fHeight = fHeight;
	}

	float BulletCapsuleCollisionShape::GetHeight() const
	{
		return m_fHeight;
	}

	void BulletCapsuleCollisionShape::SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}

	float BulletCapsuleCollisionShape::GetRadius() const
	{
		return m_fRadius;
	}
}