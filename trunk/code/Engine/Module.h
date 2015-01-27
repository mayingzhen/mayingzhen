#ifndef  _Engine_Moudule__H__
#define  _Engine_Moudule__H__

#include "WarningPrefix.h"


// RTTI
#include "Engine/RTTI/Referenced.h"
#include "Engine/RTTI/RefPtr.h"
#include "Engine/RTTI/Object.h"
#include "Engine/RTTI/RTTIClass.h"
#include "Engine/RTTI/ObjectFactory.h"
#include "Engine/RTTI/Serializable.h"
#include "Engine/RTTI/AttributeManager.h"

// Event
#include "Engine/Event/Event.h"

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
#include "Engine/Resource/Serialize/BinaryInputSerializer.h"
#include "Engine/Resource/Serialize/BinaryOutputSerializer.h"
#include "Engine/Resource/Serialize/XMLInputSerializer.h"
#include "Engine/Resource/Serialize/XMLOutputSerializer.h"


// Thread
#include "Engine/Thread/Thread.h"
#include "Engine/Thread/WorkQueue.h"

#include "Engine/RenderSystem/IRenderDevice/IndexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/HardwareBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexDeclaration.h"
#include "Engine/RenderSystem/IRenderDevice/IRenderDevice.h"
#include "Engine/RenderSystem/IRenderDevice/RenderDefine.h"
#include "Engine/RenderSystem/RenderSystem.h"
#include "Engine/RenderSystem/RenderContext.h"
#include "Engine/RenderSystem/DeviceCapabilities.h"

#include "Engine/Renderable/Renderable.h"
#include "Engine/Renderable/MeshBatch.h"
#include "Engine/Renderable/SpriteBatch.h"
#include "Engine/Renderable/MeshData.h"
#include "Engine/Renderable/LineRender.h"
#include "Engine/Renderable/ScreenQuad.h"
#include "Engine/Renderable/UnitSphere.h"

#include "Engine/RenderScheme/RenderPass.h"
#include "Engine/RenderScheme/RenderScheme.h"
#include "Engine/RenderScheme/PostProcess.h"
#include "Engine/RenderScheme/BlurPostProcess.h"
#include "Engine/RenderScheme/HDRPostProcess.h"

#include "Engine/ImageData/PixelFormat.h"
#include "Engine/Material/Texture.h"
#include "Engine/Material/RenderState.h"
#include "Engine/Material/SamplerState.h"
#include "Engine/Material/RenderState.h"
#include "Engine/Material/ShaderProgram.h"
#include "Engine/Material/MaterialParameter.h"
#include "Engine/Material/Technqiue.h"
#include "Engine/Material/Material.h"
#include "Engine/Material/MaterialData.h"
#include "Engine/Material/Uniform.h"

// Scene
#include "Engine/Scene/Component.h"
#include "Engine/Scene/SceneNode.h"
#include "Engine/Scene/SceneVisiter.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/CullNode.h"
#include "Engine/Scene/CullTree.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/meshcomponent.h"
#include "Engine/Scene/Light/Light.h"
#include "Engine/Scene/Light/LightSystem.h"
#include "Engine/Scene/Light/RenderShadowCSM.h"
#include "Engine/Scene/Particle/ParticleSystem.h"
#include "Engine/Scene/Terrain/Terrain.h"

#include "Engine/Profile/CodeTimer.h"

#include "Engine/Engine.h"

#include "WarningSuffix.h"



#endif
