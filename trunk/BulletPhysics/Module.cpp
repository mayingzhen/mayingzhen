#include "BulletPhysics/stdafx.h"
#include "BulletPhysics/Module.h"

#include "BulletPhysics/BtPhysicsDevive.hxx"
#include "BulletPhysics/BtPhysicsObject.hxx"
#include "BulletPhysics/BtPhysicsScene.hxx"
#include "BulletPhysics/BtCollisionShape.hxx"
#include "BulletPhysics/BtRigidBody.hxx"
#include "BulletPhysics/BtCharacterController.hxx"
#include "BulletPhysics/BtContactReport.hxx"


void BtPhysicsModuleInit()
{
	ma::BulletDevice* pBtPhysicsDevice = new ma::BulletDevice();
	ma::SetPhysicsDevice(pBtPhysicsDevice); 
}

void BtPhysicsModuleShutdown()
{
	ma::BulletDevice* pBtPhysicsDevice = (ma::BulletDevice*)ma::GetPhysicsDevice();
	SAFE_DELETE(pBtPhysicsDevice);
}
