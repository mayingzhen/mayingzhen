#include "BulletPhysics/stdafx.h"
#include "BulletPhysics/Module.h"

#include "BulletPhysics/BtPhysicsDevive.hxx"
#include "BulletPhysics/BtPhysicsObject.hxx"
#include "BulletPhysics/BtPhysicsScene.hxx"


void BtPhysicsModuleInit()
{
	ma::BtPhysicsDevice* pBtPhysicsDevice = new ma::BtPhysicsDevice();
	ma::SetPhysicsDevice(pBtPhysicsDevice); 
}

void BtPhysicsModuleShutdown()
{
	ma::BtPhysicsDevice* pBtPhysicsDevice = (ma::BtPhysicsDevice*)ma::GetPhysicsDevice();
	SAFE_DELETE(pBtPhysicsDevice);
}
