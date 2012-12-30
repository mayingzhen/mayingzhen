#include "BulletPhysics/BtPhysicsDevive.h"

namespace ma
{
	IPhysicsScene* BtPhysicsDevice::CreatePhysicsScene() 
	{
		m_pBtPhysicsScene = new BtPhysicsScene();
		return m_pBtPhysicsScene;
	}

	IPhysicsObject* BtPhysicsDevice::CreatePhysicsObject() 
	{
		return new BtPhysicsObject();
	}
	
	
}
