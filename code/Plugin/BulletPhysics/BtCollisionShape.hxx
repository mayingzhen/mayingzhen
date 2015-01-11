#include "BulletPhysics/BtCollisionShape.h"
#include "BulletPhysics/BulletUtil.h"

namespace ma
{
	IMPL_OBJECT(CollisionShape,Component)

	void CollisionShape::RegisterObject(Context* context)
	{
		REF_ACCESSOR_ATTRIBUTE(CollisionShape, "Position", GetPosLS, SetPosLS, Vector3, Vector3::ZERO, AM_DEFAULT);
		REF_ACCESSOR_ATTRIBUTE(CollisionShape, "Rotation", GetRotateLS, SetRotateLS, Quaternion, Quaternion::IDENTITY, AM_DEFAULT);
		REF_ACCESSOR_ATTRIBUTE(CollisionShape, "Scale", GetScaleLS, SetScaleLS, Vector3, Vector3::UNIT_SCALE, AM_DEFAULT);
	}

	void CollisionShape::SetTransformLS(const Transform& tsfLS)
	{
		m_tsfLS = tsfLS;
	}

	void CollisionShape::SetPosLS(const Vector3& vPos)
	{
		Transform tsf = m_tsfLS;
		tsf.m_vPos = vPos;
		SetTransformLS(tsf);
	}

	void CollisionShape::SetRotateLS(const Quaternion& qRot)
	{
		Transform tsf = m_tsfLS;
		tsf.m_qRot = qRot;
		SetTransformLS(tsf);
	}

	void CollisionShape::SetScaleLS(const Vector3& vScale)
	{
		Transform tsf = m_tsfLS;
		tsf.m_vScale = vScale;
		SetTransformLS(tsf);
	}

	void CollisionShape::OnAddToSceneNode(SceneNode* pGameObj)
	{
		GetPhysicsSystem()->AddPhysicsObject(pGameObj);
	}

	IMPL_OBJECT(BoxCollisionShape,CollisionShape)
	BoxCollisionShape::BoxCollisionShape()
	{
		m_vSize = Vector3(0,0,0);
		m_pBtShape = NULL;
	}

	BoxCollisionShape::~BoxCollisionShape()
	{

	}

	void BoxCollisionShape::RegisterObject(Context* context)
	{
		COPY_BASE_ATTRIBUTES(BoxCollisionShape,CollisionShape);

		REF_ACCESSOR_ATTRIBUTE(BoxCollisionShape, "Size", GetSize, SetSize, Vector3, Vector3::UNIT_SCALE, AM_DEFAULT);
	}

	void BoxCollisionShape::SetSize(const Vector3& vSize)
	{
		m_vSize = vSize;
	}

	const Vector3& BoxCollisionShape::GetSize() const
	{
		return m_vSize;
	}

	void* BoxCollisionShape::Create()
	{
		m_pBtShape = new btBoxShape( ToBulletUnit( GetSize() ) * 0.5f );
		return m_pBtShape;
	}

	IMPL_OBJECT(SphereCollisionShape,CollisionShape)

	SphereCollisionShape::SphereCollisionShape()
	{
		m_fRadius = 0;
		m_pBtShape = NULL;
	}

	SphereCollisionShape::~SphereCollisionShape()
	{

	}

	void SphereCollisionShape::RegisterObject(Context* context)
	{
		COPY_BASE_ATTRIBUTES(SphereCollisionShape,CollisionShape);

		ACCESSOR_ATTRIBUTE(SphereCollisionShape, "Radius", GetRadius, SetRadius, float, 0, AM_DEFAULT);
	}

	void SphereCollisionShape::SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}

	float SphereCollisionShape::GetRadius() const
	{
		return m_fRadius;
	}

	void* SphereCollisionShape::Create()
	{
		m_pBtShape = new btSphereShape( GetRadius() );
		return m_pBtShape;
	}

		
	IMPL_OBJECT(CapsuleCollisionShape,CollisionShape)

	CapsuleCollisionShape::CapsuleCollisionShape()
	{
		m_fRadius = 0;
		m_fHeight = 0;
		m_pBtShape = NULL;
	}

	CapsuleCollisionShape::~CapsuleCollisionShape()
	{

	}

	void CapsuleCollisionShape::RegisterObject(Context* context)
	{
		COPY_BASE_ATTRIBUTES(CapsuleCollisionShape,CollisionShape);

		ACCESSOR_ATTRIBUTE(CapsuleCollisionShape, "Radius", GetRadius, SetRadius, float, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(CapsuleCollisionShape, "Height", GetHeight, SetHeight, float, 0, AM_DEFAULT);
	}

	void CapsuleCollisionShape::SetHeight(float fHeight)
	{
		m_fHeight = fHeight;
	}

	float CapsuleCollisionShape::GetHeight() const
	{
		return m_fHeight;
	}

	void CapsuleCollisionShape::SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}

	float CapsuleCollisionShape::GetRadius() const
	{
		return m_fRadius;
	}

	void* CapsuleCollisionShape::Create()
	{
		m_pBtShape = new btCapsuleShape( GetRadius(), GetHeight() );
		return m_pBtShape;
	}

	IMPL_OBJECT(CollisionMaterial,Component)
	
	CollisionMaterial::CollisionMaterial()
	{
		m_nCollLayer = 0;
		m_friction = 0;
		m_restitution = 0;
		m_rollingFriction = 0;
	}

	void CollisionMaterial::RegisterObject(Context* context)
	{
		ACCESSOR_ATTRIBUTE(CollisionMaterial, "CollLayer", GetCollLayer, SetCollLayer, int, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(CollisionMaterial, "Friction", GetFriction, SetFriction, float, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(CollisionMaterial, "Restitution", GetRestitution, SetRestitution, float, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(CollisionMaterial, "RollingFriction", GetRollingFriction, SetRollingFriction, float, 0, AM_DEFAULT);
	}

	void CollisionMaterial::Start(btCollisionObject* pBtCollObject)
	{
		if (pBtCollObject == NULL)
			return;
		
		pBtCollObject->setFriction(m_friction);
		pBtCollObject->setRestitution(m_restitution);
		pBtCollObject->setRollingFriction(m_rollingFriction);
	}
	
}
