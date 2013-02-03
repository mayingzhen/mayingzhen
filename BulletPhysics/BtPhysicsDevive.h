#ifndef  _BtPhysicsDevice__H__
#define  _BtPhysicsDevice__H__

#include "Framework/Physics/IPhysicsDevive.h"

namespace ma
{
	class BULLETPHYSICS_API BulletDevice : public IPhysicsDevice
	{
	public:
		virtual IPhysicsScene*			CreatePhysicsScene();

		virtual IPhysicsObject*			CreatePhysicsObject();

		virtual IRigidBody*				CreateRigidBody();

		virtual IBoxCollisionShape*		CreateBoxCollisionShape();

		virtual ISphereCollisionShape*	CreateSphereCollisionShape();

		virtual ICharacterController*	CreateCharacterController();
	
	};
}

#endif
