#include "BulletPhysics/BtRigidBody.h"
#include "BulletUtil.h"
#include "BtPhysicsSystem.h"

namespace ma
{
	IMPL_OBJECT(RigidBody,Component)
	RigidBody::RigidBody()
	{
		m_pRigidBody = NULL;
		m_bUseGravity = true;
		m_bKinematic = false;
		m_rbInfo = new btRigidBody::btRigidBodyConstructionInfo(1.0f,NULL,NULL);
	}

	void RigidBody::RegisterAttribute()
	{
		ACCESSOR_ATTRIBUTE(RigidBody, "UseGravity", IsUseGravity, SetUseGravity, bool, true, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(RigidBody, "Kinematic", IsKinematic, SetKinematic, bool, false, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(RigidBody, "Mass", GetMass, SetMass, float, 1.0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(RigidBody, "LinearDamping", GetLinearDamping, SetLinearDamping, float, 0.0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(RigidBody, "AngularDamping", GetAngularDamping, SetAngularDamping, float, 0.0, AM_DEFAULT);
	}

	void RigidBody::SetUseGravity(bool bUseGravity)
	{
		if (m_pRigidBody)
		{
			if (!bUseGravity)
				m_pRigidBody->setGravity(btVector3(0,0,0));
			else
				m_pRigidBody->setGravity( GetPhysicsSystem()->GetDynamicsWorld()->getGravity() );

			m_pRigidBody->activate();
		}
		m_bUseGravity = bUseGravity;
	}

	bool RigidBody::IsUseGravity() const
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

	bool RigidBody::IsKinematic() const
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

	void RigidBody::SetKinematic(bool bKinematic)
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

	void RigidBody::SetMass(float fMass)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->setMassProps(fMass,m_rbInfo->m_localInertia);
		}
		m_rbInfo->m_mass = fMass;
	}

	float RigidBody::GetMass() const
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

	void RigidBody::SetLinearDamping(float fLinearDamping)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->setDamping(fLinearDamping,m_pRigidBody->getAngularDamping());
		}
		m_rbInfo->m_linearDamping = fLinearDamping;
	}

	float RigidBody::GetLinearDamping() const
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

	void RigidBody::SetAngularDamping(float fAngularDamping)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->setDamping(m_pRigidBody->getLinearDamping(),fAngularDamping);
		}
		m_rbInfo->m_angularDamping = fAngularDamping;
	}

	float RigidBody::GetAngularDamping() const
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

	void RigidBody::SetDeactivationTime(float fTime)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->setDeactivationTime(fTime);
		}
		else
		{
			m_fDeactivationTime = fTime;
		}
	}

	void RigidBody::SetSleepingThresholds(float fLinear,float fAngular)
	{
		if (m_pRigidBody)
		{
			m_pRigidBody->setSleepingThresholds(fLinear,fAngular);
		}
		else
		{
			m_rbInfo->m_linearSleepingThreshold = fLinear;
			m_rbInfo->m_angularSleepingThreshold = fAngular;
		}
	}

	void RigidBody::ApplyForce(Vector3 vForce)
	{		
		if (m_pRigidBody)
		{
			m_pRigidBody->applyCentralForce(ToBulletUnit(vForce));
		}
	}


	btRigidBody* RigidBody::CreatebtRigidBody(btCompoundShape* pCompoundShape,btDiscreteDynamicsWorld* pBtDynamicsWorld)
	{
		btVector3 localInertia(0,0,0);
		if (pCompoundShape)
		{
			pCompoundShape->calculateLocalInertia(m_rbInfo->m_mass,localInertia);
		}

		//btDefaultMotionState* myMotionState = new btDefaultMotionState();
		m_rbInfo->m_motionState = NULL/*myMotionState*/;
		m_rbInfo->m_collisionShape = pCompoundShape;
		m_rbInfo->m_localInertia = localInertia;
		
		m_pRigidBody = new btRigidBody(*m_rbInfo);

		pBtDynamicsWorld->addRigidBody(m_pRigidBody);	

		SetUseGravity(m_bUseGravity);
		SetKinematic(m_bKinematic);
		SetDeactivationTime(m_fDeactivationTime);
		return m_pRigidBody;
	}

	void RigidBody::SetbtRigidBody(btRigidBody* pbtRigidBody)
	{
		m_pRigidBody = pbtRigidBody;
	}

}
