#include "BulletPhysics/stdafx.h"
#include "BulletPhysics/Module.h"


#include "BulletPhysics/BtCollisionShape.hxx"
#include "BulletPhysics/BtRigidBody.hxx"
#include "BulletPhysics/BtCharacterController.hxx"
#include "BulletPhysics/BtContactReport.hxx"
#include "BulletPhysics/BtPhysicsJoint.hxx"
#include "BulletPhysics/BtPhysicsSystem.hxx"
#include "BulletPhysics/BtPhysicsThread.hxx"

using namespace ma;

#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <BulletPhysics/RTTIDecl.h>
#undef RTTI_DECL

void BtPhysicsModuleInit()
{
	PhysicsSystem* pBtPhysicsSystem = new PhysicsSystem();
	SetPhysicsSystem(pBtPhysicsSystem); 

#define RTTI_DECL(ClassType) \
	ClassType::StaticInitClass(); \
	GetObjectFactoryManager()->RegisterObjectFactory(#ClassType,Create_##ClassType); \
	ClassType::RegisterAttribute();
#include <BulletPhysics/RTTIDecl.h>
#undef RTTI_DECL

}

void BtPhysicsModuleShutdown()
{
#define RTTI_DECL(ClassType) \
	GetObjectFactoryManager()->UnRegisterObjectFactory(#ClassType,Create_##ClassType); \
	ClassType::StaticShutdownClass();
#include <BulletPhysics/RTTIDecl.h>
#undef RTTI_DECL

	PhysicsSystem* pBtPhysicsSystem = GetPhysicsSystem();
	SAFE_DELETE(pBtPhysicsSystem);
	SetPhysicsSystem(NULL);
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