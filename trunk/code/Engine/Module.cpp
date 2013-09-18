#include "Engine/stdAfx.h"
#include "Engine/Module.h"
#include "Engine/RTTI.h"


// Platform
#include "Engine/Application/Timer.hxx"
#include "Engine/Application/Platform.hxx"
#include "Engine/Application/Game.hxx"


// Event
#include "Engine/Event/Event.hxx"


// Resource
#include "Engine/Resource/DataStream.hxx"
#include "Engine/Resource/AAssetFileStream.hxx"
#include "Engine/Resource/FileStream.hxx"
#include "Engine/Resource/FileSystem.hxx"
#include "Engine/Resource/Properties.hxx"
#include "Engine/Resource/Resource.hxx"
#include "Engine/Resource/MeshData.hxx"
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


// RenderSystem
#include "Engine/RenderSystem/IRenderDevice.hxx"
#include "Engine/RenderSystem/VertexDeclaration.hxx"
#include "Engine/RenderSystem/MeshBatch.hxx"
#include "Engine/RenderSystem/RendMesh.hxx"
#include "Engine/RenderSystem/SpriteBatch.hxx"
#include "Engine/RenderSystem/Camera.hxx"
#include "Engine/RenderSystem/Light.hxx"
#include "Engine/RenderSystem/RenderSystem.hxx"
#include "Engine/RenderSystem/DeferredLight.hxx"
#include "Engine/RenderSystem/Shadow.hxx"
#include "Engine/RenderSystem/RenderThread.hxx"

#include "Engine/RenderSystem/ParticleEmitter.hxx"
#include "Engine/RenderSystem/ParticleThread.hxx"
#include "Engine/RenderSystem/ParticleManager.hxx"

// Terrain
#include "Engine/Terrain/Terrain.hxx"
#include "Engine/Terrain/TerrainSection.hxx"
#include "Engine/Terrain/TerrainLiquid.hxx"

// RenderScheme
//#include "Engine/RenderSystem/RenderQueue.hxx"
#include "Engine/RenderSystem/ShadowMapFrustum.hxx"

// Util
#include "Engine/RenderSystem/LineRender.hxx"
#include "Engine/RenderSystem/ScreenQuad.hxx"
#include "Engine/RenderSystem/UnitSphere.hxx"

#include "Engine/RenderSystem/Material/Material.hxx"
#include "Engine/RenderSystem/Material/MaterialParameter.hxx"
#include "Engine/RenderSystem/Material/ShaderProgram.hxx"
#include "Engine/RenderSystem/Material/Texture.hxx"
#include "Engine/RenderSystem/Material/Technqiue.hxx"
#include "Engine/RenderSystem/Material/MaterialManager.hxx"


// Physics
#include "Engine/Physics/IPhysicsSystem.hxx"


// script
#include "Engine/Script/IScriptSystem.hxx"

// Animation
#include "Engine/Animation/IAnimationSystem.hxx"

// UI
#include "Engine/UI/IUISystem.hxx"

// Input
#include "Engine/Input/Input.hxx"



using namespace ma;



void EngineModuleInit()
{
	EngineRTTIInit();

	MaterialManager* pMaterialMang = new MaterialManager();
	SetMaterialManager(pMaterialMang);

	RenderSystem* pRenderSystem = new RenderSystem();
	SetRenderSystem(pRenderSystem);
	
	ResourceSystem* pRsourceSystem = new ResourceSystem();
	SetResourceSystem(pRsourceSystem);
	pRsourceSystem->Init();

	Input* pInput = new Input();
	SetInput(pInput);
	pInput->Init(Platform::GetInstance().GetWindId());

	ParticleManager* pParticleMang = new ParticleManager();
	SetParticleManager(pParticleMang);

	Time* pTime = new Time();
	SetTimer(pTime);
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
	
	RenderSystem* pRenderSystem = GetRenderSystem();
	SAFE_DELETE(pRenderSystem);
	SetRenderSystem(NULL);

	MaterialManager* pMaterialMang = GetMaterialManager();
	SAFE_DELETE(pMaterialMang);
	SetMaterialManager(NULL);

	EngineRTTIShutdown();
}

