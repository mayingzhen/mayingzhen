#include "Framework/stdafx.h"
#include "Framework/Module.h"

// Node
#include "Framework/SceneNode.hxx"
#include "Framework/Scene.hxx"
#include "Framework/GameObj.hxx"
#include "Framework/Camera.hxx"

// Resource
#include "Framework/Resource.hxx"
#include "Framework/MeshRes.hxx"
#include "Framework/SkeletonRes.hxx"
#include "Framework/Texture.hxx"
#include "Framework/AnimationRes.hxx"

// Component
#include "Framework/Component.hxx"
#include "Framework/MeshComponent.hxx"
#include "Framework/SkelMeshComponent.hxx"

// physics
#include "Framework/Physics/IPhysicsDevive.hxx"
#include "Framework/Physics/CollisionComponent.hxx"
#include "Framework/Physics/RigidbodyComponent.hxx"


// Scrript
//#include "Framework/Script/ScriptEventBinder.hxx"
#include "Framework/Script/IScriptDevice.hxx"
#include "Framework/Script/EdScriptObject.hxx"
#include "Framework/Script/ScriptComponent.hxx"


namespace ma
{
	// RTTI
#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
	#include <Framework/RTTIDecl.h>
#undef RTTI_DECL


	void FrameWorkModuleInit()
	{
#define RTTI_DECL(ClassType) ClassType##::StaticInitClass();
		#include <Framework/RTTIDecl.h>
#undef RTTI_DECL

#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Framework/RTTIDecl.h>
#undef RTTI_DECL
	}

	void FrameWorkModuleShutdown()
	{

	}
}
