#include "BulletPhysics/stdafx.h"
#include "BulletPhysics/Module.h"


#include "BulletPhysics/BtCollisionShape.hxx"
#include "BulletPhysics/BtRigidBody.hxx"
#include "BulletPhysics/BtCharacterController.hxx"
#include "BulletPhysics/BtContactReport.hxx"
#include "BulletPhysics/BtPhysicsJoint.hxx"
#include "BulletPhysics/BtPhysicsSystem.hxx"
#include "BulletPhysics/BtPhysicsThread.hxx"


void BtPhysicsModuleInit()
{
	ma::BtPhysicsSystem* pBtPhysicsSystem = new ma::BtPhysicsSystem();
	ma::SetPhysicsSystem(pBtPhysicsSystem); 
}

void BtPhysicsModuleShutdown()
{
	ma::BtPhysicsSystem* pBtPhysicsSystem = (ma::BtPhysicsSystem*)ma::GetPhysicsSystem();
	SAFE_DELETE(pBtPhysicsSystem);
}


#if PLATFORM_WIN == 1
extern "C" BULLETPHYSICS_API bool dllStartPlugin()
{
	BtPhysicsModuleInit();

	return true;
}


extern "C" BULLETPHYSICS_API bool dllStopPlugin()
{
	BtPhysicsModuleShutdown();

	return true;
}
#endif