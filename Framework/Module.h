#ifndef  _FRAMEWORK_MODULE__H__
#define  _FRAMEWORK_MODULE__H__

#include "Framework/FrameworkExport.h"
#include "Framework/SceneNode.h"
#include "Framework/Scene.h"
#include "Framework/GameObj.h"
#include "Framework/Camera.h"

// Component
#include "Framework/Component.h"
#include "Framework/MeshComponent.h"
#include "Framework/SkelMeshComponent.h"

// Resource
// #include "Framework/Resource.h"
// #include "Framework/MeshRes.h"
// #include "Framework/SkeletonRes.h"
// #include "Framework/Texture.h"
// #include "Framework/AnimationRes.h"

// Physics
#include "Framework/Physics/ICharacterController.h"
#include "Framework/Physics/ICollisionShape.h"
#include "Framework/Physics/IPhysicsDevive.h"
#include "Framework/Physics/IPhysicsObject.h"
#include "Framework/Physics/IPhysicsScene.h"
#include "Framework/Physics/IRigidBody.h"
#include "Framework/Physics/RigidbodyComponent.h"
#include "Framework/Physics/CollisionComponent.h"


// script
#include "Framework/Script/IScriptDevice.h"
#include "Framework/Script/IScriptObject.h"
#include "Framework/Script/EdScriptObject.h"
#include "Framework/Script/ScriptComponent.h"
//#include "Framework/Script/ScriptEventBinder.h"

namespace ma
{
	FRAMEWORK_API void FrameWorkModuleInit();

	FRAMEWORK_API void FrameWorkModuleShutdown();
}

#endif
