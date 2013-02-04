#include "BulletPhysics/BtRigidBody.h"


namespace ma
{
	BulletRigidBody::BulletRigidBody()
	{
		m_pRigidBody = NULL;
		m_pBtDynamicsWorld = NULL;
		m_bUseGravity = true;
		m_bKinematic = false;
		m_rbInfo = new btRigidBody::btRigidBodyConstructionInfo(1.0f,NULL,NULL);
	}

	void BulletRigidBody::SetUseGravity(bool bUseGravity)
	{
		if (m_pRigidBody)
		{
			if (!bUseGravity)
				m_pRigidBody->setGravity(btVector3(0,0,0));
			else
				m_pRigidBody->setGravity(m_pBtDynamicsWorld->getGravity());

			m_pRigidBody->activate();
		}
		m_bUseGravity = bUseGravity;
	}

	bool BulletRigidBody::IsUseGravity()
	{
		if (m_pRigidBody)
		{
			int flags = m_pRigidBody->getFlags();
			return !BitFieldBase<int>::StaticGetBit(flags,BT_DISABLE_WORLD_GRAVITY);
		}
		else
		{
			return m_bUseGravity;
		}
	}

	bool BulletRigidBody::IsKinematic()
	{
		if (m_pRigidBody)
		{
			return m_pRigidBody->isKinematicObject();
		}
		else
		{
			return m_bKinematic;
		}
	}

	void BulletRigidBody::SetKinematic(bool bKinematic)
	{	
		if (m_pRigidBody)
		{

			int flags = m_pRigidBody->getCollisionFlags();
			BitFieldBase<int>::StaticSetBit(flags,btCollisionObject::CF_KINEMATIC_OBJECT,bKinematic);
			m_pRigidBody->setCollisionFlags(flags);

			m_pRigidBody->activate();
		}
		m_bKinematic = bKinematic;
	}

	void BulletRigidBody::SetMass(float fMass)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->setMassProps(fMass,m_rbInfo->m_localInertia);
		}
		m_rbInfo->m_mass = fMass;
	}

	float BulletRigidBody::GetMass()
	{
		if (m_pRigidBody)
		{
			return 1.0f / m_pRigidBody->getInvMass();
		}
		else
		{
			return m_rbInfo->m_mass;
		}
	}

	void BulletRigidBody::SetLinearDamping(float fLinearDamping)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->setDamping(fLinearDamping,m_pRigidBody->getAngularDamping());
		}
		m_rbInfo->m_linearDamping = fLinearDamping;
	}

	float BulletRigidBody::GetLinearDamping()
	{
		if (m_pRigidBody)
		{
			return m_pRigidBody->getLinearDamping();
		}
		else
		{
			return m_rbInfo->m_linearDamping;
		}
	}

	void BulletRigidBody::SetAngularDamping(float fAngularDamping)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->setDamping(m_pRigidBody->getLinearDamping(),fAngularDamping);
		}
		m_rbInfo->m_angularDamping = fAngularDamping;
	}

	float BulletRigidBody::GetAngularDamping()
	{
		if (m_pRigidBody)
		{
			return m_pRigidBody->getAngularDamping();
		}
		else
		{
			return m_rbInfo->m_angularDamping;
		}
	}

	btRigidBody* BulletRigidBody::CreatebtRigidBody(btCompoundShape* pCompoundShape,btDiscreteDynamicsWorld* pBtDynamicsWorld)
	{
		btVector3 localInertia(0,0,0);
		if (pCompoundShape)
		{
			pCompoundShape->calculateLocalInertia(m_rbInfo->m_mass,localInertia);
		}

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		m_rbInfo->m_motionState = myMotionState;
		m_rbInfo->m_collisionShape = pCompoundShape;
		m_rbInfo->m_localInertia = localInertia;
		
		m_pRigidBody = new btRigidBody(*m_rbInfo);

		pBtDynamicsWorld->addRigidBody(m_pRigidBody);	

		m_pBtDynamicsWorld = pBtDynamicsWorld;

		SetUseGravity(m_bUseGravity);
		SetKinematic(m_bKinematic);
		return m_pRigidBody;
	}

	void BulletRigidBody::SetbtRigidBody(btRigidBody* pbtRigidBody)
	{
		m_pRigidBody = pbtRigidBody;
	}

}
