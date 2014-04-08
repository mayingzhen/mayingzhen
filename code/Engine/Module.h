#ifndef  _Engine_Moudule__H__
#define  _Engine_Moudule__H__

#include "WarningPrefix.h"

#include "Engine/Export.h"

// Event
#include "Engine/Event/Event.h"

// RTTI
#include "Engine/RTTI/Object.h"
#include "Engine/RTTI/RTTIClass.h"
#include "Engine/RTTI/ObjectFactory.h"

#include "Engine/Time/Timer.h"

// Resource
#include "Engine/Resource/FileInfo.h"
#include "Engine/Resource/Stream.h"
#include "Engine/Resource/MemoryStream.h"
#include "Engine/Resource/ZipArchive.h"
#include "Engine/Resource/FileArchive.h"
#include "Engine/Resource/AAssetArchive.h"
#include "Engine/Resource/ArchiveManager.h"
#include "Engine/Resource/Properties.h"
#include "Engine/Resource/Resource.h"
#include "Engine/Resource/ResourceSystem.h"
#include "Engine/Resource/Serialize/tinyxml/tinyxml.h"
#include "Engine/Resource/Serialize/Serializer.h"
#include "Engine/Resource/Serialize/BinaryInputArchive.h"
#include "Engine/Resource/Serialize/BinaryOutputArchive.h"
#include "Engine/Resource/Serialize/StringConverter.h"
#include "Engine/Resource/Serialize/XMLInputArchive.h"
#include "Engine/Resource/Serialize/XMLOutputArchive.h"


// Entity System
#include "Engine/EntitySystem/Component.h"
#include "Engine/EntitySystem/GameObj.h"
//#include "Engine/EntitySystem/Scene.h"
#include "Engine/EntitySystem/SceneNode.h"
#include "Engine/EntitySystem/SceneVisiter.h"
#include "Engine/EntitySystem/EntitySystem.h"
#include "Engine/EntitySystem/CullTree.h"

// Terrain
#include "Engine/Terrain/ITerrain.h"


// Physics
#include "Engine/Physics/ICharacterController.h"
#include "Engine/Physics/ICollisionShape.h"
#include "Engine/Physics/IPhysicsSystem.h"
#include "Engine/Physics/IPhysicsObject.h"
#include "Engine/Physics/IRigidBody.h"
#include "Engine/Physics/IPhysicsJoint.h"

// script
#include "Engine/Script/IScriptSystem.h"
#include "Engine/Script/IScriptObject.h"


// Animation
#include "Engine/Animation/IAnimationObject.h"
#include "Engine/Animation/IAnimationSystem.h"
#include "Engine/Animation/IAnimationSet.h"
#include "Engine/Animation/IAction.h"


// UI
#include "Engine/UI/IUISystem.h"


// Input
#include "Engine/Input/Input.h"


// Thread
#include "Engine/Thread/Thread.h"


#include "Engine/Profile/CodeTimer.h"

#include "WarningSuffix.h"


ENGINE_API void EngineModuleInit();

ENGINE_API void EngineModuleShutdown();

#endif
