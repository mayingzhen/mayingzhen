#ifndef  _BtPhysicsDevice__H__
#define  _BtPhysicsDevice__H__

#include "BulletPhysics/BtPhysicsScene.h"
#include "BulletPhysics/BtPhysicsObject.h"

namespace ma
{
	class BULLETPHYSICS_API BtPhysicsDevice : public IPhysicsDevice
	{

	public:
		virtual IPhysicsScene* CreatePhysicsScene() {return new BtPhysicsScene();}
	
		virtual IPhysicsObject* CreatePhysicsObject() {return new BtPhysicsObject();}
	};
}


#endif