#pragma once

#include "WarningPrefix.h"


// RTTI
#include "Engine/RTTI/Object.h"
#include "Engine/RTTI/Serializable.h"
#include "Engine/RTTI/ObjectFactory.h"
#include "Engine/RTTI/AttributeManager.h"

#include "Engine/Time/Timer.h"
#include "Engine/Time/Animatable.h"

// Resource
#include "Engine/Resource/Stream.h"
#include "Engine/Resource/MemoryStream.h"
#include "Engine/Resource/FileStream.h"
#include "Engine/Resource/ZipArchive.h"
#include "Engine/Resource/FileArchive.h"
#include "Engine/Resource/AAssetArchive.h"
#include "Engine/Resource/ArchiveManager.h"
#include "Engine/Resource/Resource.h"
#include "Engine/Resource/XmlFile.h"
#include "Engine/Resource/ResourceSystem.h"

// Thread
#include "Engine/Thread/MSemaphore.h"
#include "Engine/Thread/JobScheduler.h"


// TaskScheduler
//#include "Engine/TaskScheduler/MTScheduler.h"
//#include "Engine/TaskScheduler/task_scheduler.h"

#include "Engine/RenderSystem/IRenderDevice/IndexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/HardwareBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/TransientBuffer.h"
#include "Engine/RenderSystem/IRenderDevice/VertexDeclaration.h"
#include "Engine/RenderSystem/IRenderDevice/IRenderDevice.h"
#include "Engine/RenderSystem/IRenderDevice/RenderDefine.h"
#include "Engine/RenderSystem/IRenderDevice/RenderState.h"
#include "Engine/RenderSystem/RenderSystem.h"
#include "Engine/RenderSystem/RenderContext.h"
#include "Engine/RenderSystem/DeviceCapabilities.h"
#include "Engine/RenderSystem/RenderCommand.h"
#include "Engine/RenderSystem/PrefilterCube.h"

#include "Engine/Renderable/Renderable.h"
#include "Engine/Renderable/MeshRenderable.h"
#include "Engine/Renderable/SkinMeshRenderable.h"
#include "Engine/Renderable/MeshBatch.h"
#include "Engine/Renderable/MeshData.h"
#include "Engine/Renderable/LineRender.h"
#include "Engine/Renderable/ScreenQuad.h"
#include "Engine/Renderable/UnitSphere.h"

#include "Engine/RenderScheme/RenderPass.h"
#include "Engine/RenderScheme/RenderScheme.h"
#include "Engine/RenderScheme/PostProcess.h"

#include "Engine/ImageData/PixelFormat.h"
#include "Engine/ImageData/ImageData.h"
#include "Engine/Material/Texture.h"
#include "Engine/Material/ShaderProgram.h"
#include "Engine/Material/MaterialParameter.h"
#include "Engine/Material/Technqiue.h"
#include "Engine/Material/Material.h"
#include "Engine/Material/Uniform.h"
#include "Engine/Material/UniformAnimation.h"

// Scene
#include "Engine/Scene/Component.h"
#include "Engine/Scene/SceneNode.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/CullNode.h"
#include "Engine/Scene/CullTree.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/MeshComponent.h"
#include "Engine/Scene/SkinMeshComponent.h"
#include "Engine/Scene/Light/Light.h"
#include "Engine/Scene/Light/DirectonalLight.h"


#include "Engine/Scene/Terrain/Terrain.h"

#include "Engine/Scene/SkyBox.h"

#include "Engine/Scene/ParticleComponent.h"

#include "Engine/Engine.h"

#include "WarningSuffix.h"


