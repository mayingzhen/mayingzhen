#include "BtPhysicsJoint.h"

namespace ma
{
	void PhysicsJoint::RegisterAttribute()
	{

	}

	void PhysicsJoint::SetEnabled(bool enabled)
	{
		m_bEnabled = enabled;
		if (m_pConstraint)
			m_pConstraint->setEnabled(enabled);
	}

	void PhysicsJoint::SetBreakingImpulse(float impulse)
	{ 
		m_fBreakingImpulse = impulse; 
		if (m_pConstraint) 
			m_pConstraint->setBreakingImpulseThreshold(impulse);
	}

	void PhysicsJoint::Start()
	{

	}

	void PhysicsJoint::Stop()
	{

	}

	PhysicsGenericJoint::PhysicsGenericJoint()
	{
		m_pPhyObjB = NULL;
		m_pConstraint = NULL;
		m_bEnabled = true;

		m_vAngularLowerLimit = Vector3(0,0,0);
		m_vAngularUpperLimit = Vector3(0,0,0);
		m_vLinearLowerLimit = Vector3(0,0,0);
		m_vLinearUpperLimit = Vector3(0,0,0);

		m_bAngularLowerLimit = false;
		m_bAngularUpperLimit = false;
		m_bLinearLowerLimit = false;
		m_bLinearUpperLimit = false;
	}

	void PhysicsGenericJoint::RegisterAttribute()
	{

	}
	
	void	PhysicsGenericJoint::SetAngularLowerLimit(const Vector3& limit)
	{
		m_bAngularLowerLimit= true;
		m_vAngularLowerLimit= limit;
		if (m_pConstraint)
		{	
			((btGeneric6DofConstraint*)m_pConstraint)->setAngularLowerLimit(ToBulletUnit(limit));
		}
	}

	void	PhysicsGenericJoint::SetAngularUpperLimit(const Vector3& limit)
	{
		m_bAngularUpperLimit = true;
		m_vAngularUpperLimit = limit;
		if (m_pConstraint)
		{	
			((btGeneric6DofConstraint*)m_pConstraint)->setAngularUpperLimit(ToBulletUnit(limit));
		}
	}

	void	PhysicsGenericJoint::SetLinearLowerLimit(const Vector3& limit)
	{	
		m_bLinearLowerLimit = true;
		m_vLinearLowerLimit = limit;
		if (m_pConstraint)
		{	
			((btGeneric6DofConstraint*)m_pConstraint)->setLinearLowerLimit(ToBulletUnit(limit));
		}
	}

	void	PhysicsGenericJoint::SetLinearUpperLimit(const Vector3& limit)
	{
		m_bLinearUpperLimit = true;
		m_vLinearUpperLimit = limit;
		if (m_pConstraint)
		{	
			((btGeneric6DofConstraint*)m_pConstraint)->setLinearUpperLimit(ToBulletUnit(limit));
		}
	}

	btRigidBody* GetbtRigidBody(SceneNode* pPhyObj)
	{
		if (pPhyObj == NULL)
			return NULL;

		RigidBody* pRigidBody = pPhyObj->GetTypeComponent<RigidBody>();
		if (pRigidBody == NULL)
			return NULL;

		return pRigidBody->GetbtRigidBody();
	}

	void	PhysicsGenericJoint::Start()
	{
		btRigidBody* pRigidBodyA = GetbtRigidBody(m_pSceneNode);
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

	void PhysicsGenericJoint::Stop()
	{

	}

	PhysicsHingeJoint::PhysicsHingeJoint()
	{
		m_pConstraint = NULL;
		m_pPhyObjB = NULL;
	}

	void PhysicsHingeJoint::RegisterAttribute()
	{

	}

	void PhysicsHingeJoint::SetLimits(float minAngle, float maxAngle, float bounciness)
	{
		m_fMinAngle = minAngle;
		m_fMaxAngle = maxAngle;
		m_fBounciness = bounciness;
		if (m_pConstraint)
		{
			((btHingeConstraint*)m_pConstraint)->setLimit(minAngle, maxAngle, 0.9f, 0.3f, bounciness);
		}
	}

	void PhysicsHingeJoint::Start()
	{

	}

	void PhysicsHingeJoint::Stop()
	{

	}

}

