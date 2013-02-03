#include "BulletPhysics/BtPhysicsDevive.h"

namespace ma
{
	IPhysicsScene*	BulletDevice::CreatePhysicsScene()
	{
		return new BulletScene;
	}

	IPhysicsObject*	BulletDevice::CreatePhysicsObject()
	{
		return new BulletPhysicsObject;
	}

	IRigidBody*			BulletDevice::CreateRigidBody()
	{
		return new BulletRigidBody;
	}

	IBoxCollisionShape*	BulletDevice::CreateBoxCollisionShape()
	{
		return new BulletBoxCollisionShape;
	}

	ISphereCollisionShape* BulletDevice::CreateSphereCollisionShape()
	{
		return new BulletSphereCollisionShape;
	}

	ICharacterController* BulletDevice::CreateCharacterController()
	{
		return new BulletCharacterController;
	}
}

