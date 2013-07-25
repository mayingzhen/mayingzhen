#ifndef  _BtPhysicsDevice__H__
#define  _BtPhysicsDevice__H__

#include "Engine/Physics/IPhysicsDevive.h"

namespace ma
{
	class BULLETPHYSICS_API BulletDevice : public IPhysicsDevice
	{
	public:
		BulletDevice();

		virtual IPhysicsScene*			CreatePhysicsScene();

		virtual void					DestoryPhysicsScene(IPhysicsScene* pPhysicsScene);

// 		virtual IPhysicsObject*			CreatePhysicsObject();
// 
// 		virtual IRigidBody*				CreateRigidBody();
// 
// 		virtual IBoxCollisionShape*		CreateBoxCollisionShape();
// 
// 		virtual ISphereCollisionShape*	CreateSphereCollisionShape();
// 
// 		virtual ICharacterController*	CreateCharacterController();

		BulletScene*					GetPhysicsScene() {return m_pPhyscicsScene;}

	private:
		BulletScene*					m_pPhyscicsScene;
	
	};
}

#endif
