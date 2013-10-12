#include "EntitySystem/stdAfx.h"
#include "EntitySystem/Module.h"

#include "EntitySystem.hxx"
#include "Component/Component.hxx"
#include "Component/AnimComponent.hxx"
#include "Component/MeshComponent.hxx"
#include "Component/ParticleComponent.hxx"
#include "Component/ScriptComponent.hxx"
#include "Component/LightComponent.hxx"

// Physics
#include "Component/RigidbodyComponent.hxx"
#include "Component/CollisionComponent.hxx"
#include "Component/CharaControlComponent.hxx"
#include "Component/JointComponent.hxx"


#include "GameObj.hxx"
#include "Scene.hxx"
#include "SceneNode.hxx"
#include "Util.hxx"



using namespace ma;

// RTTI
#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <EntitySystem/RTTIDecl.h>
#undef RTTI_DECL

#define RTTI_DECL(ClassType) Object* Create_##ClassType(void* arg) { return new ClassType((GameObject*)arg);}
#include <EntitySystem/CompRTTIDecl.h>
#undef RTTI_DECL


void EntitySystemModuleInit()
{
	#define RTTI_DECL(ClassType) ClassType::StaticInitClass();
	#include <EntitySystem/RTTIDecl.h>
	#include <EntitySystem/CompRTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <EntitySystem/RTTIDecl.h>
	#include <EntitySystem/CompRTTIDecl.h>
	#undef RTTI_DECL

	EntitySystem* pEntitySystem = new EntitySystem();
	SetEntitySystem(pEntitySystem);
	pEntitySystem->Init();
}

void EntitySystemModuleShutdown()
{
	EntitySystem* pEntitySystem = GetEntitySystem();
	pEntitySystem->ShoutDown();
	SAFE_DELETE(pEntitySystem)
	SetEntitySystem(NULL);

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().UnRegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <EntitySystem/RTTIDecl.h>
	#include <EntitySystem/CompRTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ClassType::StaticShutdownClass();
	#include <EntitySystem/RTTIDecl.h>
	#include <EntitySystem/CompRTTIDecl.h>
	#undef RTTI_DECL
}

