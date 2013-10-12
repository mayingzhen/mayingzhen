#ifndef  _EntitySystem_Moudule__H__
#define  _EntitySystem_Moudule__H__


#include "EntitySystem/Export.h"

#include "EntitySystem/GameObj.h"
#include "EntitySystem/Scene.h"
#include "EntitySystem/SceneNode.h"
#include "EntitySystem/SceneVisiter.h"
#include "EntitySystem/Util.h"
#include "EntitySystem/EntitySystem.h"

#include "EntitySystem/Component/Component.h"
#include "EntitySystem/Component/AnimComponent.h"
#include "EntitySystem/Component/MeshComponent.h"
#include "EntitySystem/Component/ParticleComponent.h"
#include "EntitySystem/Component/ScriptComponent.h"
#include "EntitySystem/Component/LightComponent.h"

// Physics
#include "EntitySystem/Component/RigidbodyComponent.h"
#include "EntitySystem/Component/CharaControlComponent.h"
#include "EntitySystem/Component/CollisionComponent.h"
#include "EntitySystem/Component/JointComponent.h"


ENTITYSYSTEM_API void EntitySystemModuleInit();

ENTITYSYSTEM_API void EntitySystemModuleShutdown();

#endif
