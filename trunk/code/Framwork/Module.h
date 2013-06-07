#ifndef  _Framwork_Moudule__H__
#define  _Framwork_Moudule__H__


#include "Framwork/Export.h"

#include "Framwork/Component/Component.h"
#include "Framwork/Component/SkelMeshComponent.h"
#include "Framwork/Component/MeshComponent.h"
#include "Framwork/Component/RigidbodyComponent.h"
#include "Framwork/Component/CollisionComponent.h"
#include "Framwork/Component/ScriptComponent.h"


#include "Framwork/GameObj.h"
#include "Framwork/Scene.h"
#include "Framwork/SceneNode.h"





FRAMWORK_API void FramworkModuleInit();

FRAMWORK_API void FramworkModuleShutdown();

#endif
