#include "BtPhysicsJoint.h"

namespace ma
{
	BulletPhysicsGenericJoint::BulletPhysicsGenericJoint(BulletPhysicsObject* pPhyObjA,BulletPhysicsObject* pPhyObjB)
	{
		m_pPhyObjA = pPhyObjA;
		m_pPhyObjB = pPhyObjB;
		m_pConstraint = NULL;
		m_bEnabled = true;
		TransformSetIdentity(&m_AtsfLS);
		TransformSetIdentity(&m_BtsfLS);

		m_vAngularLowerLimit = Vector3(0,0,0);
		m_vAngularUpperLimit = Vector3(0,0,0);
		m_vLinearLowerLimit = Vector3(0,0,0);
		m_vLinearUpperLimit = Vector3(0,0,0);

		m_bAngularLowerLimit = false;
		m_bAngularUpperLimit = false;
		m_bLinearLowerLimit = false;
		m_bLinearUpperLimit = false;
	}
	
	void	BulletPhysicsGenericJoint::SetAngularLowerLimit(const Vector3& limit)
	{
		m_bAngularLowerLimit= true;
		m_vAngularLowerLimit= limit;
		if (m_pConstraint)
		{	
			((btGeneric6DofConstraint*)m_pConstraint)->setAngularLowerLimit(ToBulletUnit(limit));
		}
	}

	void	BulletPhysicsGenericJoint::SetAngularUpperLimit(const Vector3& limit)
	{
		m_bAngularUpperLimit = true;
		m_vAngularUpperLimit = limit;
		if (m_pConstraint)
		{	
			((btGeneric6DofConstraint*)m_pConstraint)->setAngularUpperLimit(ToBulletUnit(limit));
		}
	}

	void	BulletPhysicsGenericJoint::SetLinearLowerLimit(const Vector3& limit)
	{	
		m_bLinearLowerLimit = true;
		m_vLinearLowerLimit = limit;
		if (m_pConstraint)
		{	
			((btGeneric6DofConstraint*)m_pConstraint)->setLinearLowerLimit(ToBulletUnit(limit));
		}
	}

	void	BulletPhysicsGenericJoint::SetLinearUpperLimit(const Vector3& limit)
	{
		m_bLinearUpperLimit = true;
		m_vLinearUpperLimit = limit;
		if (m_pConstraint)
		{	
			((btGeneric6DofConstraint*)m_pConstraint)->setLinearUpperLimit(ToBulletUnit(limit));
		}
	}

	btRigidBody*	GetbtRigidBody(BulletPhysicsObject* pPhyObj)
	{
		//ASSERT(pPhyObj && pPhyObj->GetbtCollisionObject());
		if (pPhyObj == NULL || pPhyObj->GetbtCollisionObject() == NULL)
			return NULL;

		btRigidBody* pRigidBody = btRigidBody::upcast(pPhyObj->GetbtCollisionObject());
		ASSERT(pRigidBody);
		if (pRigidBody == NULL)
			return NULL;

		return pRigidBody;
	}

	void	BulletPhysicsGenericJoint::Start()
	{
		btRigidBody* pRigidBodyA = GetbtRigidBody(m_pPhyObjA);
		btRigidBody* pRigidBodyB = GetbtRigidBody(m_pPhyObjB);
		ASSERT(pRigidBodyA);
		if (pRigidBodyA == NULL)
			return;

		btTransform frameInA = ToBulletUnit(m_AtsfLS);

		if (pRigidBodyB)
		{
			btTransform frameInB = ToBulletUnit(m_BtsfLS);

			m_pConstraint = new btGeneric6DofConstraint(*pRigidBodyA, *pRigidBodyB, frameInA, frameInB, true);
		}
		else
		{
			m_pConstraint = new btGeneric6DofConstraint(*pRigidBodyA, frameInA, true);
		}
	
		if (m_bLinearLowerLimit)
			SetLinearLowerLimit(m_vLinearLowerLimit);
		
		if (m_bLinearUpperLimit)
			SetLinearUpperLimit(m_vLinearUpperLimit);
		
		if (m_bAngularLowerLimit)
			SetAngularLowerLimit(m_vAngularLowerLimit);
		
		if (m_bAngularUpperLimit)
			SetAngularUpperLimit(m_vAngularUpperLimit);
	}

	void BulletPhysicsGenericJoint::Stop()
	{

	}

	BulletPhysicsHingeJoint::BulletPhysicsHingeJoint(BulletPhysicsObject* pPhyObjA,BulletPhysicsObject* pPhyObjB)
	{
		m_pConstraint = NULL;
		m_pPhyObjA = pPhyObjA;
		m_pPhyObjB = pPhyObjB;
	}

	void BulletPhysicsHingeJoint::SetLimits(float minAngle, float maxAngle, float bounciness)
	{
		m_fMinAngle = minAngle;
		m_fMaxAngle = maxAngle;
		m_fBounciness = bounciness;
		if (m_pConstraint)
		{
			((btHingeConstraint*)m_pConstraint)->setLimit(minAngle, maxAngle, 0.9f, 0.3f, bounciness);
		}
	}

	void BulletPhysicsHingeJoint::Start()
	{

	}

	void BulletPhysicsHingeJoint::Stop()
	{

	}

}

