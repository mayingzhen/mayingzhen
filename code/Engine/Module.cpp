#include "Engine/stdAfx.h"
#include "Engine/Module.h"
#include "Engine/RTTI.h"

// RTTI
#include "Engine/RTTI/Object.hxx"
#include "Engine/RTTI/ObjectFactory.hxx"
#include "Engine/RTTI/Attribute.hxx"
#include "Engine/RTTI/AttributeManager.hxx"
#include "Engine/RTTI/Serializable.hxx"

#include "Engine/Time/Timer.hxx"
#include "Engine/Time/Animatable.hxx"


#include "Engine/Thread/MSemaphore.hxx"
#include "Engine/Thread/JobScheduler.hxx"



// Resource
#include "Engine/Resource/Stream.hxx"
#include "Engine/Resource/MemoryStream.hxx"
#include "Engine/Resource/FileStream.hxx"
#include "Engine/Resource/FileArchive.hxx"
#include "Engine/Resource/ZipDataStream.hxx"
#include "Engine/Resource/ZipArchive.hxx"
#include "Engine/Resource/ArchiveManager.hxx"
#include "Engine/Resource/Resource.hxx"
#include "Engine/Resource/XmlFile.hxx"
#include "Engine/Resource/DataThread.hxx"


#ifdef PLATFORM_ANDROID
#include "Engine/Resource/AAssetStream.hxx"
#include "Engine/Resource/AAssetArchive.hxx"
#endif // PLATFORM_ANDROID



#include "Engine/ImageData/PixelFormat.hxx"
#include "Engine/ImageData/ImageData.hxx"
#include "Engine/ImageData/ImageCodec.hxx"
#include "Engine/ImageData/DDSCodec.hxx"
#include "Engine/ImageData/ETC1Codec.hxx"
#include "Engine/ImageData/PVRTCCodec.hxx"
#include "Engine/ImageData/PVRTCDecompress.hxx"

// Renderable
#include "Engine/Renderable/Renderable.hxx"
#include "Engine/Renderable/MeshRenderable.hxx"
#include "Engine/Renderable/SkinMeshRenderable.hxx"
#include "Engine/Renderable/MeshBatch.hxx"
#include "Engine/Renderable/MeshData.hxx"
#include "Engine/Renderable/LineRender.hxx"
#include "Engine/Renderable/ScreenQuad.hxx"
#include "Engine/Renderable/UnitSphere.hxx"


#include "Engine/RenderSystem/RenderSystem.hxx"
#include "Engine/RenderSystem/RenderThread.hxx"
#include "Engine/RenderSystem/BatchRenderable.hxx"
#include "Engine/RenderSystem/InstanceRenderable.hxx"
#include "Engine/RenderSystem/BatchTerrainRenderable.hxx"
#include "Engine/RenderSystem/RenderQueue.hxx"
#include "Engine/RenderSystem/RenderContext.hxx"
#include "Engine/RenderSystem/DeviceCapabilities.hxx"
#include "Engine/RenderSystem/IRenderDevice/IRenderDevice.hxx"
#include "Engine/RenderSystem/IRenderDevice/HardwareBuffer.hxx"
#include "Engine/RenderSystem/IRenderDevice/TransientBuffer.hxx"
#include "Engine/RenderSystem/IRenderDevice/VertexDeclaration.hxx"
#include "Engine/RenderSystem/IRenderDevice/RenderState.hxx"
#include "Engine/RenderSystem/PrefilterCube.hxx"

#include "Engine/Material/Material.hxx"
#include "Engine/Material/MaterialParameter.hxx"
#include "Engine/Material/ShaderProgram.hxx"
#include "Engine/Material/ShaderManager.hxx"
#include "Engine/Material/Texture.hxx"
#include "Engine/Material/TextureManager.hxx"
#include "Engine/Material/Technqiue.hxx"
#include "Engine/Material/ParameterManager.hxx"
#include "Engine/Material/Uniform.hxx"
#include "Engine/Material/UniformAnimation.hxx"

#include "Engine/RenderScheme/DeferredShadingPass.hxx"
#include "Engine/RenderScheme/DeferredShadowPass.hxx"
#include "Engine/RenderScheme/RenderScheme.hxx"
#include "Engine/RenderScheme/BlurPostProcess.hxx"
#include "Engine/RenderScheme/HDRPostProcess.hxx"
#include "Engine/RenderScheme/SMAAPostProcess.hxx"
#include "Engine/RenderScheme/AlchemyAo.hxx"

// Scene
#include "Engine/Scene/Component.hxx"
#include "Engine/Scene/RenderComponent.hxx"
#include "Engine/Scene/MeshComponent.hxx"
#include "Engine/Scene/SkinMeshComponent.hxx"
#include "Engine/Scene/SceneNode.hxx"
#include "Engine/Scene/Scene.hxx"
#include "Engine/Scene/OctreeNode.hxx"
#include "Engine/Scene/Octree.hxx"
#include "Engine/Scene/Camera.hxx"
#include "Engine/Scene/ParallelCull.hxx"

#if defined(_WIN32)
#include "Engine/Scene/OcclusionCulling/AABBoxRasterizer.hxx"
#include "Engine/Scene/OcclusionCulling/AABBoxRasterizerSSE.hxx"
#include "Engine/Scene/OcclusionCulling/AABBoxRasterizerSSEST.hxx"
#include "Engine/Scene/OcclusionCulling/DepthBufferRasterizer.hxx"
#include "Engine/Scene/OcclusionCulling/DepthBufferRasterizerSSE.hxx"
#include "Engine/Scene/OcclusionCulling/DepthBufferRasterizerSSEST.hxx"
#include "Engine/Scene/OcclusionCulling/HelperSSE.hxx"
#include "Engine/Scene/OcclusionCulling/TransformedAABBoxSSE.hxx"
#include "Engine/Scene/OcclusionCulling/TransformedModelSSE.hxx"
#include "Engine/Scene/OcclusionCulling/TransformedMeshSSE.hxx"
#endif
 
// Light
#include "Engine/Scene/Light/Light.hxx"
#include "Engine/Scene/Light/ShadowMapFrustum.hxx"
#include "Engine/Scene/Light/DirectonalLight.hxx"
#include "Engine/Scene/Light/PoissonDiskGen.hxx"


//Terrain
#include "Engine/Scene/Terrain/Terrain.hxx"
#include "Engine/Scene/Terrain/TerrainTrunk.hxx"
#include "Engine/Scene/Terrain/TerrainRenderable.hxx"

#include "Engine/Profile/CodeTimer.hxx"


#include "Engine/Engine.hxx"


