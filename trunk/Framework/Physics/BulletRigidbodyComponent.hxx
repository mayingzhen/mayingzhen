#include "BulletPhysics/BulletRigidbodyComponent.h"


namespace ma
{

	BulletRigidBodyComponent::BulletRigidBodyComponent()
	{
		m_collisionFlags = 0;
		m_bUseGravity = true;
		m_bKinematic = false;
		m_pRigudBody = NULL;
	}

	BulletRigidBodyComponent::~BulletRigidBodyComponent()
	{

	}

	void BulletRigidBodyComponent::SetMass(float fMass)
	{
		if (m_pRigudBody)
		{
			m_pRigudBody->setMassProps(fMass, m_rbInfo.m_localInertia);
		}
		else
		{
			m_rbInfo.m_mass = fMass;
		}
	}

	float BulletRigidBodyComponent::GetMass()
	{
		if (m_pRigudBody)
		{
			1.0f / m_pRigudBody->getInvMass();
		}
		else
		{
			return m_rbInfo.m_mass;
		}
	}

	// 	void BulletActor::SetUseGravity(bool bUseGravity)
	// 
	// 	bool BulletActor::IsUseGravity();

	void BulletRigidBodyComponent::SetKinematic(bool bKinematic)
	{
		if (m_pRigudBody)
		{
			int flags = m_pRigudBody->getCollisionFlags();
			BitField::StaticSetBit(flags,btCollisionObject::CF_KINEMATIC_OBJECT,bKinematic);
			m_pRigudBody->setCollisionFlags(flags);
		}
		else
		{
			BitField::StaticSetBit(m_collisionFlags,btCollisionObject::CF_KINEMATIC_OBJECT,bKinematic);
		}
	}

	bool BulletRigidBodyComponent::IsKinematic()
	{
		if (m_pRigudBody)
		{
			return m_pRigudBody->isKinematicObject();
		}
		else
		{
			return BitField::StaticGetBit(m_collisionFlags,btCollisionObject::CF_KINEMATIC_OBJECT);	
		}
	}


	void BulletRigidBodyComponent::SetLinearDamping(float fLinearDamping)
	{
		if (m_pRigudBody)
		{
			m_pRigudBody->setDamping(fLinearDamping,m_pRigudBody->getAngularDamping());
		}
		else
		{
			m_rbInfo.m_linearDamping = fLinearDamping;
		}
	}

	float BulletRigidBodyComponent::GetLinearDamping()
	{
		if (m_pRigudBody)
		{
			return m_pRigudBody->getLinearDamping();
		}
		else
		{
			return m_rbInfo.m_linearDamping;
		}
	}

	void BulletRigidBodyComponent::SetAngularDamping(float fAngularDamping)
	{
		if (m_pRigudBody)
		{
			m_pRigudBody->setDamping(m_pRigudBody->getLinearDamping(),fAngularDamping);
		}
		else
		{
			return m_rbInfo.m_linearDamping;
		}
	}

	float BulletRigidBodyComponent::GetAngularDamping()
	{
		if (m_pRigudBody)
		{
			return m_pRigudBody->getAngularDamping();
		}
		else
		{
			return m_rbInfo.m_angularDamping;
		}
	}

	void	BulletRigidBodyComponent::SyncFromScene()
	{
		GameObject* pGameObject = this->GetGameObject();
		if (m_pRigudBody && IsKinematic() && pGameObject)
		{
			xmNodeTransform tsfWS = pGameObject->GetTransformWS();
			m_pRigudBody->getMotionState()->setWorldTransform( ToBulletUnit(tsfWS) );
		}
	}

	void	BulletRigidBodyComponent::SyncToScene()
	{
		GameObject* pGameObject = this->GetGameObject();
		if (m_pRigudBody && !IsKinematic() && pGameObject)
		{
			btTransform btTSFWS;
			m_pRigudBody->getMotionState()->getWorldTransform(btTSFWS);
			pGameObject->SetTransformWS( ToMaUnit(btTSFWS) );
		}
	}

	void BulletRigidBodyComponent::Start()
	{
		btDiscreteDynamicsWorld* pBulletWorld = GetBulletWorld(this);
		assert(pBulletWorld);
		if (pBulletWorld == NULL)
			return;

		m_rbInfo.m_motionState = new btDefaultMotionState();
		btRigidBody* body = new btRigidBody(m_rbInfo);
		pBulletWorld->addRigidBody(body);
	}

	virtual void BulletRigidBodyComponent::Stop()
	{
		btDiscreteDynamicsWorld* pBulletWorld = GetBulletWorld(this);
		assert(pBulletWorld);
		if (pBulletWorld == NULL)
			return;

		pBulletWorld->removeRigidBody(m_pRigudBody);
		SAFE_DELETE(m_pRigudBody);
	}
}

