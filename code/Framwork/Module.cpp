#include "Framwork/stdAfx.h"
#include "Framwork/Module.h"


#include "Framwork/Component/Component.hxx"
#include "Framwork/Component/SkelMeshComponent.hxx"
#include "Framwork/Component/MeshComponent.hxx"
#include "Framwork/Component/RigidbodyComponent.hxx"
#include "Framwork/Component/CollisionComponent.hxx"
#include "Framwork/Component/ScriptComponent.hxx"


#include "Framwork/GameObj.hxx"
#include "Framwork/Scene.hxx"
#include "Framwork/SceneNode.hxx"




using namespace ma;

// RTTI
#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <Framwork/RTTIDecl.h>
#undef RTTI_DECL


void FramworkModuleInit()
{
	#define RTTI_DECL(ClassType) ClassType::StaticInitClass();
	#include <Framwork/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Framwork/RTTIDecl.h>
	#undef RTTI_DECL
}

void FramworkModuleShutdown()
{
	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().UnRegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Framwork/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ClassType::StaticShutdownClass();
	#include <Framwork/RTTIDecl.h>
	#undef RTTI_DECL
}

