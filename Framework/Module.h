#ifndef  _FRAMEWORK_MODULE__H__
#define  _FRAMEWORK_MODULE__H__

#include "Framework/FrameworkExport.h"
#include "Framework/Object.h"
#include "Framework/SceneNode.h"
#include "Framework/Scene.h"
#include "Framework/GameObj.h"
#include "Framework/Camera.h"

// Component
#include "Framework/Component.h"
#include "Framework/MeshComponent.h"
#include "Framework/SkelMeshComponent.h"

// Resource
#include "Framework/Resource.h"
#include "Framework/MeshRes.h"
#include "Framework/SkeletonRes.h"
#include "Framework/Texture.h"
#include "Framework/AnimationRes.h"

// Physics
#include "Framework/Physics/RigidbodyComponent.h"
#include "Framework/Physics/CollisionComponent.h"

// Input
#include "Framework/Input/Input.h"



FRAMEWORK_API void FrameWorkModuleInit();

FRAMEWORK_API void FrameWorkModuleShutdown();



#endif