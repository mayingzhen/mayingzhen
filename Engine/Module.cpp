#include "Engine/stdAfx.h"
#include "Engine/Module.h"


// Application
#include "Engine/Application/Input.hxx"
#include "Engine/Application/Timer.hxx"
#include "Engine/Application/Application.hxx"
#include "Engine/Application/ApplicationWin.hxx"


// Resource
#include "Engine/Resource/Resource.hxx"
#include "Engine/Resource/MeshData.hxx"
#include "Engine/Resource/AnimationData.hxx"
#include "Engine/Resource/SkeletonData.hxx"
#include "Engine/Resource/ResourceBuilder.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxml.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinystr.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxmlerror.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxmlparser.hxx"
#include "Engine/Resource/Serialize/SerializeListener.hxx"
#include "Engine/Resource/Serialize/BinaryInputArchive.hxx"
#include "Engine/Resource/Serialize/BinaryOutputArchive.hxx"
#include "Engine/Resource/Serialize/XMLInputArchive.hxx"
#include "Engine/Resource/Serialize/XMLOutputArchive.hxx"


// RenderSystem
#include "Engine/RenderSystem/IRenderDevice.hxx"
#include "Engine/RenderSystem/IRendMesh.hxx"
#include "Engine/RenderSystem/MeshComponent.hxx"


// Animation
#include "Engine/Animation/IAnimationDevice.hxx"
#include "Engine/Animation/SkelMeshComponent.hxx"


// Physics
#include "Engine/Physics/IPhysicsDevive.hxx"
#include "Engine/Physics/RigidbodyComponent.hxx"
#include "Engine/Physics/CollisionComponent.hxx"


// script
#include "Engine/Script/IScriptDevice.hxx"
#include "Engine/Script/EdScriptObject.hxx"
#include "Engine/Script/ScriptComponent.hxx"
//#include "Engine/Script/ScriptEventBinder.hxx"


// Scene
#include "Engine/Scene/Camera.hxx"
#include "Engine/Scene/Component.hxx"
#include "Engine/Scene/GameObj.hxx"
#include "Engine/Scene/Light.hxx"
#include "Engine/Scene/Scene.hxx"
#include "Engine/Scene/SceneNode.hxx"


using namespace ma;

// RTTI
#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <Engine/RTTIDecl.h>
#undef RTTI_DECL


void EngineModuleInit()
{
	#define RTTI_DECL(ClassType) ClassType##::StaticInitClass();
	#include <Engine/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Engine/RTTIDecl.h>
	#undef RTTI_DECL
}

void EngineModuleShutdown()
{
	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().UnRegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Engine/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ClassType##::StaticShutdownClass();
	#include <Engine/RTTIDecl.h>
	#undef RTTI_DECL
}

