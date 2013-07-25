#include "BulletPhysics/BtPhysicsDevive.h"

namespace ma
{
	BulletDevice::BulletDevice()
	{
		m_pPhyscicsScene = NULL;
	}

	IPhysicsScene*	BulletDevice::CreatePhysicsScene()
	{
		ASSERT(m_pPhyscicsScene == NULL);
		m_pPhyscicsScene = new BulletScene;
		return m_pPhyscicsScene;
	}

	void BulletDevice::DestoryPhysicsScene(IPhysicsScene* pPhysicsScene)
	{
		ASSERT(m_pPhyscicsScene == pPhysicsScene)
		SAFE_DELETE(pPhysicsScene);
		m_pPhyscicsScene = NULL;
	}

// 	IPhysicsObject*	BulletDevice::CreatePhysicsObject()
// 	{
// 		return new BulletPhysicsObject;
// 	}
// 
// 	IRigidBody*			BulletDevice::CreateRigidBody()
// 	{
// 		return new BulletRigidBody;
// 	}
// 
// 	IBoxCollisionShape*	BulletDevice::CreateBoxCollisionShape()
// 	{
// 		return new BulletBoxCollisionShape;
// 	}
// 
// 	ISphereCollisionShape* BulletDevice::CreateSphereCollisionShape()
// 	{
// 		return new BulletSphereCollisionShape;
// 	}

// 	ICharacterController* BulletDevice::CreateCharacterController()
// 	{
// 		return new BulletCharacterController;
// 	}
}

