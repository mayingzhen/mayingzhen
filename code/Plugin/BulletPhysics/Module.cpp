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

#define RTTI_DECL(ClassType) RefPtr<Object> Create_##ClassType() { return new ClassType();}
#include <BulletPhysics/RTTIDecl.h>
#undef RTTI_DECL

void BtPhysicsModuleInit()
{
	//g_pPhysicsSystem = new PhysicsSystem();

#define RTTI_DECL(ClassType) \
	GetObjectFactoryManager()->RegisterObjectFactory(#ClassType,Create_##ClassType); \
	ClassType::RegisterAttribute();
#include <BulletPhysics/RTTIDecl.h>
#undef RTTI_DECL

}

void BtPhysicsModuleShutdown()
{
#define RTTI_DECL(ClassType) \
	GetObjectFactoryManager()->UnRegisterObjectFactory(#ClassType,Create_##ClassType); 
	#include <BulletPhysics/RTTIDecl.h>
#undef RTTI_DECL

	SAFE_DELETE(g_pPhysicsSystem);
}


#if PLATFORM_WIN == 1
extern "C" bool dllStartPlugin()
{
	BtPhysicsModuleInit();

	return true;
}


extern "C" bool dllStopPlugin()
{
	BtPhysicsModuleShutdown();

	return true;
}
#endif