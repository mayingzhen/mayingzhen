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

// 	Vector3 getWorldCenterOfMass(const Node* node)
// 	{
// 		GP_ASSERT(node);
// 
// 		const BoundingSphere& sphere = node->getBoundingSphere();
// 		if (!(sphere.center.isZero() && sphere.radius == 0))
// 		{
// 			// The world-space center of mass is the sphere's center.
// 			return sphere.center;
// 		}
// 
// 		// Warn the user that the node has no bounding volume.
// 		GP_WARN("Node %s' has no bounding volume - center of mass is defaulting to local coordinate origin.", node->getId());
// 
// 		Vector3 center;
// 		node->getWorldMatrix().transformPoint(&center);
// 		return center;
// 	}
// 
// 	Vector3 centerOfMassMidpoint(const Node* a, const Node* b)
// 	{
// 		GP_ASSERT(a);
// 		GP_ASSERT(b);
// 
// 		Vector3 tA, tB;
// 		a->getWorldMatrix().getTranslation(&tA);
// 		b->getWorldMatrix().getTranslation(&tB);
// 
// 		tA = getWorldCenterOfMass(a);
// 		tB = getWorldCenterOfMass(b);
// 
// 		Vector3 d(tA, tB);
// 		d.scale(0.5f);
// 		Vector3 c(tA);
// 		c.add(d);
// 
// 		return c;
// 	}
// 
// 	btTransform PhysicsConstraint::getTransformOffset(const Node* node, const Vector3& origin)
// 	{
// 		GP_ASSERT(node);
// 
// 		// Create a translation matrix that translates to the given origin.
// 		Matrix m;
// 		Matrix::createTranslation(origin, &m);
// 
// 		// Calculate the translation and rotation offset to the rigid body
// 		// by transforming the translation matrix above into the rigid body's
// 		// local space (multiply by the inverse world matrix and extract components).
// 		Matrix mi;
// 		node->getWorldMatrix().invert(&mi);
// 		mi.multiply(m);
// 
// 		Quaternion r;
// 		mi.getRotation(&r);
// 
// 		Vector3 t;
// 		mi.getTranslation(&t);
// 
// 		Vector3 s;
// 		node->getWorldMatrix().getScale(&s);
// 
// 		t.x *= s.x;
// 		t.y *= s.y;
// 		t.z *= s.z;
// 
// 		t = offsetByCenterOfMass(node, t);
// 
// 		return btTransform(BQ(r), BV(t));
// 	}

	void	BulletPhysicsGenericJoint::Create()
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

	void BulletPhysicsHingeJoint::Create()
	{

	}

}

