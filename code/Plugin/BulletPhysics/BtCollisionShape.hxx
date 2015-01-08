#include "BulletPhysics/BtCollisionShape.h"
#include "BulletPhysics/BulletUtil.h"

namespace ma
{	
	BulletBoxCollisionShape::BulletBoxCollisionShape()
	{
		m_vSize = Vector3(0,0,0);
		m_pBtShape = NULL;
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

	void BulletBoxCollisionShape::Serialize(Serializer& sl, const char* pszLable /* = "IBoxCollisionShape" */)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(m_tsfLS,"tsfLS");
		sl.Serialize(m_vSize,"vSize");

		sl.EndSection();
	}

	void* BulletBoxCollisionShape::Create()
	{
		m_pBtShape = new btBoxShape( ToBulletUnit( GetSize() ) * 0.5f );
		return m_pBtShape;
	}

	BulletSphereCollisionShape::BulletSphereCollisionShape()
	{
		m_fRadius = 0;
		m_pBtShape = NULL;
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

	void BulletSphereCollisionShape::Serialize(Serializer& sl, const char* pszLable /* = "IBoxCollisionShape" */)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_tsfLS,"tsfLS");
		sl.Serialize(m_fRadius,"fRadius");

		sl.EndSection();
	}

	void* BulletSphereCollisionShape::Create()
	{
		m_pBtShape = new btSphereShape( GetRadius() );
		return m_pBtShape;
	}


	BulletCapsuleCollisionShape::BulletCapsuleCollisionShape()
	{
		m_fRadius = 0;
		m_fHeight = 0;
		m_pBtShape = NULL;
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

	void BulletCapsuleCollisionShape::Serialize(Serializer& sl, const char* pszLable /* = "IBoxCollisionShape" */)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_tsfLS,"tsfLS");
		sl.Serialize(m_fRadius,"fRadius");
		sl.Serialize(m_fHeight,"fHeight");

		sl.EndSection();
	}

	void* BulletCapsuleCollisionShape::Create()
	{
		m_pBtShape = new btCapsuleShape( GetRadius(), GetHeight() );
		return m_pBtShape;
	}

	IMPL_OBJECT(BulletCollisionMaterial,ICollisionMaterial)
	
	BulletCollisionMaterial::BulletCollisionMaterial()
	{
		m_nCollLayer = 0;
		m_friction = 0;
		m_restitution = 0;
		m_rollingFriction = 0;
	}

	void BulletCollisionMaterial::Serialize(Serializer& sl, const char* pszLable/* = "ICollisionMaterial"*/)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_nCollLayer,"CollLayer");
		sl.Serialize(m_friction,"friction");
		sl.Serialize(m_restitution,"restitution");
		sl.Serialize(m_rollingFriction,"rollingFriction");

		sl.EndSection();
	}

	void BulletCollisionMaterial::Start(btCollisionObject* pBtCollObject)
	{
		if (pBtCollObject == NULL)
			return;
		
		pBtCollObject->setFriction(m_friction);
		pBtCollObject->setRestitution(m_restitution);
		pBtCollObject->setRollingFriction(m_rollingFriction);
	}
	
}
