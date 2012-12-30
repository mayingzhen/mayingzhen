#ifndef  _BtPhysicsDevice__H__
#define  _BtPhysicsDevice__H__

#include "BulletPhysics/BtPhysicsScene.h"
#include "BulletPhysics/BtPhysicsObject.h"

namespace ma
{
	class BULLETPHYSICS_API BtPhysicsDevice : public IPhysicsDevice
	{

	public:
		virtual IPhysicsScene* CreatePhysicsScene(); 
	
		virtual IPhysicsObject* CreatePhysicsObject();

		BtPhysicsScene* GetBtPhysicsScene() {return m_pBtPhysicsScene;}
	
	private:
		BtPhysicsScene* m_pBtPhysicsScene;
	};
}


#endif