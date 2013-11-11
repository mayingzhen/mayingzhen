#include "Engine/stdAfx.h"
#include "Engine/Module.h"
#include "Engine/RTTI.h"


// Platform
#include "Engine/Application/Timer.hxx"
#include "Engine/Application/Platform.hxx"
#include "Engine/Application/Game.hxx"


// Event
#include "Engine/Event/Event.hxx"

#include "Engine/Thread/Thread.hxx"

// Resource
#include "Engine/Resource/DataStream.hxx"
#include "Engine/Resource/AAssetFileStream.hxx"
#include "Engine/Resource/FileStream.hxx"
#include "Engine/Resource/FileSystem.hxx"
#include "Engine/Resource/Properties.hxx"
#include "Engine/Resource/Resource.hxx"
#include "Engine/Resource/ResourceSystem.hxx"
#include "Engine/Resource/DataThread.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxml.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinystr.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxmlerror.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxmlparser.hxx"
#include "Engine/Resource/Serialize/Serializer.hxx"
#include "Engine/Resource/Serialize/BinaryInputArchive.hxx"
#include "Engine/Resource/Serialize/BinaryOutputArchive.hxx"
#include "Engine/Resource/Serialize/XMLInputArchive.hxx"
#include "Engine/Resource/Serialize/XMLOutputArchive.hxx"


// Terrain
#include "Engine/Terrain/ITerrain.hxx"


// Physics
#include "Engine/Physics/ICollisionShape.hxx"
#include "Engine/Physics/ICharacterController.hxx"
#include "Engine/Physics/IRigidBody.hxx"
#include "Engine/Physics/IPhysicsJoint.hxx"
#include "Engine/Physics/IPhysicsSystem.hxx"
//#include "Engine/Physics/Ragdoll.hxx"


// script
#include "Engine/Script/IScriptObject.hxx"
#include "Engine/Script/IScriptSystem.hxx"

// Animation
#include "Engine/Animation/IAnimationObject.hxx"
#include "Engine/Animation/IAnimationSystem.hxx"

// UI
#include "Engine/UI/IUISystem.hxx"

// Input
#include "Engine/Input/Input.hxx"

// Entity System
#include "Engine/EntitySystem/Component.hxx"
#include "Engine/EntitySystem/GameObj.hxx"
#include "Engine/EntitySystem/Scene.hxx"
#include "Engine/EntitySystem/SceneNode.hxx"
#include "Engine/EntitySystem/SceneVisiter.hxx"
#include "Engine/EntitySystem/EntitySystem.hxx"
#include "Engine/EntitySystem/Util.hxx"




using namespace ma;



void EngineModuleInit()
{
	EngineRTTIInit();

	ResourceSystem* pRsourceSystem = new ResourceSystem();
	SetResourceSystem(pRsourceSystem);
	pRsourceSystem->Init();

	Input* pInput = new Input();
	SetInput(pInput);
	pInput->Init(Platform::GetInstance().GetWindId());

	Time* pTime = new Time();
	SetTimer(pTime);

	Scene*	pSceneSystem = new Scene();
	SetSceneSystem(pSceneSystem);

	EntitySystem* pEntitySystem = new EntitySystem();
	SetEntitySystem(pEntitySystem);
	pEntitySystem->Init();
}

void EngineModuleShutdown()
{
	Time* pTime = GetTimer();
	SAFE_DELETE(pTime);
	SetTimer(NULL);

	Input* pInput = GetInput();
	pInput->Shutdown();
	SAFE_DELETE(pInput);
	SetInput(NULL);

	ResourceSystem* pRsourceSystem = GetResourceSystem();
	pRsourceSystem->ShoutDown(); 
	SAFE_DELETE(pRsourceSystem);
	SetResourceSystem(NULL);
	
	EngineRTTIShutdown();
}

