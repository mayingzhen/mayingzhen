#include "Framework/Physics/RigidbodyComponent.h"


namespace ma
{




	RigidBodyComponent::RigidBodyComponent()
	{
	}

	RigidBodyComponent::~RigidBodyComponent()
	{

	}

	void RigidBodyComponent::SetMass(float fMass)
	{
		//if (m_pPhysicsObj)
		//{
		//	m_pPhysicsObj->setMassProps(fMass, m_rbInfo.m_localInertia);
		//}
		//else
		//{
			m_rbInfo.m_fMass = fMass;
		//}
	}

	float RigidBodyComponent::GetMass()
	{
		//if (m_pPhysicsObj)
		//{
		//	1.0f / m_pRigudBody->getInvMass();
		//}
		//else
		//{
			return m_rbInfo.m_fMass;
		//}
	}

	// 	void BulletActor::SetUseGravity(bool bUseGravity)
	// 
	// 	bool BulletActor::IsUseGravity();

	void RigidBodyComponent::SetKinematic(bool bKinematic)
	{
		//if (m_pRigudBody)
		//{
		//	UINT flags = m_pRigudBody->getCollisionFlags();
		//	BitField::StaticSetBit(flags,btCollisionObject::CF_KINEMATIC_OBJECT,bKinematic);
		//	m_pRigudBody->setCollisionFlags(flags);
		//}
		//else
		//{
		m_rbInfo.m_bKinematic = bKinematic;
			//BitField::StaticSetBit(m_rbInfo.m_collisionFlags,btCollisionObject::CF_KINEMATIC_OBJECT,bKinematic);
		//}
	}

	bool RigidBodyComponent::IsKinematic()
	{
		//if (m_pRigudBody)
		//{
		//	return m_pRigudBody->isKinematicObject();
		//}
		//else
		//{
		return m_rbInfo.m_bKinematic;
			//return BitField::StaticGetBit(m_rbInfo.m_collisionFlags,btCollisionObject::CF_KINEMATIC_OBJECT);	
		//}
	}

	void RigidBodyComponent::SetLinearDamping(float fLinearDamping)
	{
		//if (m_pRigudBody)
		//{
		//	m_pRigudBody->setDamping(fLinearDamping,m_pRigudBody->getAngularDamping());
		//}
		//else
		//{
		m_rbInfo.m_fLinearDamping = fLinearDamping;
		//	m_rbInfo.m_linearDamping = fLinearDamping;
		//}
	}

	float RigidBodyComponent::GetLinearDamping()
	{
		//if (m_pRigudBody)
		//{
		//	return m_pRigudBody->getLinearDamping();
		//}
		//else
		//{
		return m_rbInfo.m_fLinearDamping;
		//	return m_rbInfo.m_linearDamping;
		//}
	}

	void RigidBodyComponent::SetAngularDamping(float fAngularDamping)
	{
		//if (m_pRigudBody)
		//{
		//	m_pRigudBody->setDamping(m_pRigudBody->getLinearDamping(),fAngularDamping);
		//}
		//else
		//{
		m_rbInfo.m_fAngularDamping = fAngularDamping;
		//	m_rbInfo.m_angularDamping = fAngularDamping;
		//}
	}

	float RigidBodyComponent::GetAngularDamping()
	{
		//if (m_pRigudBody)
		//{
		//	return m_pRigudBody->getAngularDamping();
		//}
		//else
		//{
		return m_rbInfo.m_fAngularDamping;
		//	return m_rbInfo.m_angularDamping;
		//}
	}

	void RigidBodyComponent::Update()
	{
		
	}

	void RigidBodyComponent::Start()
	{
		if ( GetGameObject() == NULL)
			return;

		m_pPhysicsObj = GetGameObject()->GetPhyscisObject();
		if (m_pPhysicsObj == NULL)
		{
			IPhysicsDevice* pPhysicsDevice = GetPhysicsDevice();
			if (pPhysicsDevice)
			{
				m_pPhysicsObj = pPhysicsDevice->CreatePhysicsObject();
				GetGameObject()->SetPhyscisObject(m_pPhysicsObj);
			}
		}

		if (m_pPhysicsObj == NULL)
			return;

		m_pPhysicsObj->SetRigidBody(m_rbInfo);
	}

	void RigidBodyComponent::Stop()
	{
		//btDiscreteDynamicsWorld* pBulletWorld = GetBulletWorld(this);
		//assert(pBulletWorld);
		//if (pBulletWorld == NULL)
		//	return;

		//pBulletWorld->removeRigidBody(m_pRigudBody);
		//SAFE_DELETE(m_pRigudBody);
	}
}

