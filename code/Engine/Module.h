#ifndef  _Engine_Moudule__H__
#define  _Engine_Moudule__H__

#include "WarningPrefix.h"

#include "Engine/Export.h"

// Event
#include "Engine/Event/Event.h"

// Platform
#include "Engine/Application/Timer.h"
#include "Engine/Application/Platform.h"
#include "Engine/Application/Game.h"



// Resource
#include "Engine/Resource/Stream.h"
#include "Engine/Resource/DataStream.h"
#include "Engine/Resource/AAssetFileStream.h"
#include "Engine/Resource/FileStream.h"
#include "Engine/Resource/FileSystem.h"
#include "Engine/Resource/Properties.h"
#include "Engine/Resource/Resource.h"
#include "Engine/Resource/MeshData.h"
#include "Engine/Resource/ResourceSystem.h"
#include "Engine/Resource/Serialize/tinyxml/tinyxml.h"
#include "Engine/Resource/Serialize/Serializer.h"
#include "Engine/Resource/Serialize/BinaryInputArchive.h"
#include "Engine/Resource/Serialize/BinaryOutputArchive.h"
#include "Engine/Resource/Serialize/StringConverter.h"
#include "Engine/Resource/Serialize/XMLInputArchive.h"
#include "Engine/Resource/Serialize/XMLOutputArchive.h"


// RenderSystem
#include "Engine/RenderSystem/RenderDefine.h"
#include "Engine/RenderSystem/IRenderDevice.h"
#include "Engine/RenderSystem/IndexBuffer.h"
#include "Engine/RenderSystem/VertexBuffer.h"
#include "Engine/RenderSystem/VertexDeclaration.h"
#include "Engine/RenderSystem/Renderable.h"
#include "Engine/RenderSystem/RendMesh.h"
#include "Engine/RenderSystem/MeshBatch.h"
#include "Engine/RenderSystem/SpriteBatch.h"
#include "Engine/RenderSystem/RenderTarget.h"
#include "Engine/RenderSystem/Camera.h"
#include "Engine/RenderSystem/Light.h"
#include "Engine/RenderSystem/RenderSystem.h"
#include "Engine/RenderSystem/DeferredLight.h"
#include "Engine/RenderSystem/Shadow.h"
#include "Engine/RenderSystem/RenderThread.h"
#include "Engine/RenderSystem/RenderQueue.h"

#include "Engine/RenderSystem/ParticleEmitter.h"
#include "Engine/RenderSystem/ParticleSystem.h"

// Terrain
#include "Engine/Terrain/ITerrain.h"


// RenderScheme
//#include "Engine/RenderSystem/RenderQueue.h"
#include "Engine/RenderSystem/ShadowMapFrustum.h"

// Util
#include "Engine/RenderSystem/LineRender.h"
#include "Engine/RenderSystem/ScreenQuad.h"
#include "Engine/RenderSystem/UnitSphere.h"

// Material
#include "Engine/RenderSystem/Material/Material.h"
#include "Engine/RenderSystem/Material/MaterialParameter.h"
#include "Engine/RenderSystem/Material/ShaderProgram.h"
#include "Engine/RenderSystem/Material/Texture.h"
#include "Engine/RenderSystem/Material/Technqiue.h"


// Physics
#include "Engine/Physics/ICharacterController.h"
#include "Engine/Physics/ICollisionShape.h"
#include "Engine/Physics/IPhysicsSystem.h"
#include "Engine/Physics/IPhysicsObject.h"
#include "Engine/Physics/IRigidBody.h"
#include "Engine/Physics/IPhysicsJoint.h"
//#include "Engine/Physics/Ragdoll.h"

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


#include "WarningSuffix.h"


ENGINE_API void EngineModuleInit();

ENGINE_API void EngineModuleShutdown();

#endif
