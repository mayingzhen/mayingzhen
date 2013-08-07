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
//#include "Engine/Resource/AnimationData.hxx"
//#include "Engine/Resource/SkeletonData.hxx"
//#include "Engine/Resource/Texture.hxx"
#include "Engine/Resource/ResourceMangager.hxx"
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
#include "Engine/RenderSystem/ParticleEmitter.hxx"
#include "Engine/RenderSystem/SpriteBatch.hxx"
#include "Engine/RenderSystem/Camera.hxx"
#include "Engine/RenderSystem/Light.hxx"

// Terrain
#include "Engine/Terrain/Terrain.hxx"
#include "Engine/Terrain/TerrainSection.hxx"
#include "Engine/Terrain/TerrainLiquid.hxx"

// RenderScheme
#include "Engine/RenderSystem/RenderQueue.hxx"
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


// Animation
//#include "Engine/Animation/IAnimationSystem.hxx"


// Physics
#include "Engine/Physics/IPhysicsDevive.hxx"


// script
#include "Engine/Script/IScriptDevice.hxx"
//#include "Engine/Script/EdScriptObject.hxx"
//#include "Engine/Script/ScriptEventBinder.hxx"



// Input
#include "Engine/Input/Input.hxx"
//#include "Engine/Input/Joystick.hxx"
//#include "Engine/Input/Gamepad.hxx"


#include "Engine/Thread/DataThread.hxx"
//#include "Engine/Thread/RenderThread.hxx"


using namespace ma;



void EngineModuleInit()
{
	EngineRTTIInit();

	//DataThread* pDtaThread = new DataThread(0,.0);
	//SetDataThread(pDtaThread);
}

void EngineModuleShutdown()
{
	EngineRTTIShutdown();

	//delete GetDataThread();
	//SetDataThread(NULL);
}

