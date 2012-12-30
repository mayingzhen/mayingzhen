#ifndef  _FRAMEWORK_MODULE__H__
#define  _FRAMEWORK_MODULE__H__

#include "Framework/FrameworkExport.h"
#include "Framework/Object.h"
#include "Framework/SceneNode.h"
#include "Framework/Scene.h"
#include "Framework/GameObj.h"
#include "Framework/Camera.h"
#include "Framework/MeshComponent.h"
#include "Framework/SkelMeshComponent.h"

// Physics
#include "Framework/Physics/RigidbodyComponent.h"
#include "Framework/Physics/CollisionComponent.h"



FRAMEWORK_API void FrameWorkModuleInit();

FRAMEWORK_API void FrameWorkModuleShutdown();



#endif