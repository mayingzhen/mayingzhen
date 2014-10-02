#include "Engine/stdAfx.h"
#include "Engine/Module.h"
#include "Engine/RTTI.h"

// RTTI
#include "Engine/RTTI/RTTIClass.hxx"
#include "Engine/RTTI/Object.hxx"
#include "Engine/RTTI/ObjectFactory.hxx"


#include "Engine/Time/Timer.hxx"

// Event
#include "Engine/Event/Event.hxx"

#include "Engine/Thread/Thread.hxx"
#include "Engine/Thread/WorkQueue.hxx"

// Resource
#include "Engine/Resource/Stream.hxx"
#include "Engine/Resource/MemoryStream.hxx"
#include "Engine/Resource/FileStream.hxx"
#include "Engine/Resource/FileArchive.hxx"
#include "Engine/Resource/ZipDataStream.hxx"
#include "Engine/Resource/ZipArchive.hxx"
#include "Engine/Resource/ArchiveManager.hxx"
#include "Engine/Resource/Properties.hxx"
#include "Engine/Resource/Resource.hxx"
#include "Engine/Resource/ResourceSystem.hxx"
#include "Engine/Resource/DataThread.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxml.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinystr.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxmlerror.hxx"
#include "Engine/Resource/Serialize/tinyxml/tinyxmlparser.hxx"
#include "Engine/Resource/Serialize/BinarySerializer.hxx"
#include "Engine/Resource/Serialize/BinaryInputSerializer.hxx"
#include "Engine/Resource/Serialize/BinaryOutputSerializer.hxx"
#include "Engine/Resource/Serialize/XMLSerializer.hxx"
#include "Engine/Resource/Serialize/XMLInputSerializer.hxx"
#include "Engine/Resource/Serialize/XMLOutputSerializer.hxx"

#ifdef PLATFORM_ANDROID
#include "Engine/Resource/AAssetStream.hxx"
#include "Engine/Resource/AAssetArchive.hxx"
#endif // PLATFORM_ANDROID


// Physics
#include "Engine/Physics/ICollisionShape.hxx"
#include "Engine/Physics/ICharacterController.hxx"
#include "Engine/Physics/IRigidBody.hxx"
#include "Engine/Physics/IPhysicsJoint.hxx"
#include "Engine/Physics/IPhysicsSystem.hxx"


// script
#include "Engine/Script/IScriptObject.hxx"
#include "Engine/Script/IScriptSystem.hxx"

// UI
#include "Engine/UI/IUISystem.hxx"

// Input
#include "Engine/Input/Input.hxx"

// Scene
#include "Engine/Scene/Component.hxx"
#include "Engine/Scene/RenderComponent.hxx"
#include "Engine/Scene/SceneNode.hxx"
#include "Engine/Scene/NodeTransform.hxx"
#include "Engine/Scene/Scene.hxx"
#include "Engine/Scene/OctreeNode.hxx"
#include "Engine/Scene/Octree.hxx"

#include "Engine/Profile/CodeTimer.hxx"



using namespace ma;



void EngineModuleInit()
{
	EngineRTTIInit();

	CodeTimerManager* pCodeTimeMng = new CodeTimerManager();
	SetCodeTimeMng(pCodeTimeMng);

	ArchiveManager* pArchiveMang = new ArchiveManager();
	SetArchiveManager(pArchiveMang);

	ResourceSystem* pRsourceSystem = new ResourceSystem();
	SetResourceSystem(pRsourceSystem);
	pRsourceSystem->Init();

	Time* pTime = new Time();
	SetTimer(pTime);

// 	WorkQueue* pWorkQueue = new WorkQueue();
// 	SetWorkQueue(pWorkQueue);
// 	pWorkQueue->CreateThreads(3);

	//Scene* pEntitySystem = new Scene();
	//SetEntitySystem(pEntitySystem);
	//pEntitySystem->Init();
}

void EngineModuleShutdown()
{
	Time* pTime = GetTimer();
	SAFE_DELETE(pTime);
	SetTimer(NULL);

	//Scene* pEntitySystem = GetEntitySystem();
	//pEntitySystem->ShoutDown();
	//SAFE_DELETE(pEntitySystem);
	//SetEntitySystem(NULL);

	ResourceSystem* pRsourceSystem = GetResourceSystem();
	pRsourceSystem->ShoutDown(); 
	SAFE_DELETE(pRsourceSystem);
	SetResourceSystem(NULL);

	ArchiveManager* pArchiveManager = GetArchiveMananger();
	SAFE_DELETE(pArchiveManager);
	SetArchiveManager(NULL);
	
	EngineRTTIShutdown();
}

