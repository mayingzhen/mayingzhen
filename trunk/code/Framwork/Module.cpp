#include "Framwork/stdAfx.h"
#include "Framwork/Module.h"

#include "Framwork/EntitySystem.hxx"
#include "Framwork/Component/Component.hxx"
#include "Framwork/Component/AnimComponent.hxx"
#include "Framwork/Component/MeshComponent.hxx"
#include "Framwork/Component/ParticleComponent.hxx"
#include "Framwork/Component/ScriptComponent.hxx"

// Physics
#include "Framwork/Component/RigidbodyComponent.hxx"
#include "Framwork/Component/CollisionComponent.hxx"
#include "Framwork/Component/CharaControlComponent.hxx"
#include "Framwork/Component/JointComponent.hxx"


#include "Framwork/GameObj.hxx"
#include "Framwork/Scene.hxx"
#include "Framwork/SceneNode.hxx"
#include "Framwork/Util.hxx"



using namespace ma;

// RTTI
#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <Framwork/RTTIDecl.h>
#undef RTTI_DECL

#define RTTI_DECL(ClassType) Object* Create_##ClassType(void* arg) { return new ClassType((GameObject*)arg);}
#include <Framwork/CompRTTIDecl.h>
#undef RTTI_DECL


void FramworkModuleInit()
{
	#define RTTI_DECL(ClassType) ClassType::StaticInitClass();
	#include <Framwork/RTTIDecl.h>
	#include <Framwork/CompRTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Framwork/RTTIDecl.h>
	#include <Framwork/CompRTTIDecl.h>
	#undef RTTI_DECL

	EntitySystem* pEntitySystem = new EntitySystem();
	SetEntitySystem(pEntitySystem);
	pEntitySystem->Init();
}

void FramworkModuleShutdown()
{
	EntitySystem* pEntitySystem = GetEntitySystem();
	pEntitySystem->ShoutDown();
	SAFE_DELETE(pEntitySystem)
	SetEntitySystem(NULL);

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().UnRegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Framwork/RTTIDecl.h>
	#include <Framwork/CompRTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ClassType::StaticShutdownClass();
	#include <Framwork/RTTIDecl.h>
	#include <Framwork/CompRTTIDecl.h>
	#undef RTTI_DECL
}

