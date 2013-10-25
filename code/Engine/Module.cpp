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
#include "Engine/RenderSystem/RenderQueue.hxx"
#include "Engine/RenderSystem/RenderObject.hxx"

#include "Engine/RenderSystem/ParticleEmitter.hxx"
#include "Engine/RenderSystem/ParticleThread.hxx"
#include "Engine/RenderSystem/ParticleSystem.hxx"

// Terrain
#include "Engine/Terrain/ITerrain.hxx"
#include "Engine/Terrain/Terrain.hxx"
#include "Engine/Terrain/TerrainSection.hxx"
//#include "Engine/Terrain/TerrainLiquid.hxx"

// RenderScheme
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
#include "Engine/RenderSystem/Material/SamplerState.hxx"


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

	ParticleSystem* pParticleMang = new ParticleSystem();
	SetParticleSystem(pParticleMang);
	pParticleMang->Init();

	Terrain* pTerrain = new Terrain();
	SetTerrain(pTerrain);

	Time* pTime = new Time();
	SetTimer(pTime);

	EntitySystem* pEntitySystem = new EntitySystem();
	SetEntitySystem(pEntitySystem);
	pEntitySystem->Init();
}

void EngineModuleShutdown()
{
	Time* pTime = GetTimer();
	SAFE_DELETE(pTime);
	SetTimer(NULL);

	Terrain* pTerrain = (Terrain*)GetTerrain();
	SAFE_DELETE(pTerrain);
	SetTerrain(NULL);

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
