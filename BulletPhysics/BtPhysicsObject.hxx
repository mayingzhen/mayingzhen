#include "BulletPhysics/BtPhysicsObject.h"
#include "BulletPhysics/BulletUtil.h"

namespace ma
{
	void BtPhysicsObject::Start()
	{

	}

	void BtPhysicsObject::SetRigidBody(const RigidBodyInfo& info)
	{

	}

	void BtPhysicsObject::AddBoxCollision(const BoxCollisionInfo& info)
	{

	}

	void BtPhysicsObject::SetTransformWS(const maNodeTransform& tsfWS)
	{
		if (m_pRigidBody == NULL)
			return;

		m_pRigidBody->setWorldTransform( ToBulletUnit(tsfWS) );
	}

	maNodeTransform BtPhysicsObject::GetTransformWS() const
	{
		if (m_pRigidBody == NULL)
		{
			maNodeTransform tsfWS;
			maTransformSetIdentity(&tsfWS);
			return tsfWS;
		}

		return ToMaUnit( m_pRigidBody->getWorldTransform() );
	}
}