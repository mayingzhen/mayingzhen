#include "Render/stdAfx.h"

#include "Render/Module.h"



#include "Render/IRenderDevice/IRenderDevice.hxx"
#include "Render/IRenderDevice/VertexDeclaration.hxx"

#include "Render/Renderable/MeshBatch.hxx"
#include "Render/Renderable/RendMesh.hxx"
#include "Render/Renderable/SpriteBatch.hxx"
#include "Render/Renderable/RenderObject.hxx"
#include "Render/Renderable/MeshData.hxx"

#include "Render/RenderSystem/Camera.hxx"
#include "Render/RenderSystem/RenderSystem.hxx"
#include "Render/RenderSystem/RenderThread.hxx"
#include "Render/RenderSystem/RenderQueue.hxx"


#include "Render/Particle/ParticleEmitter.hxx"
#include "Render/Particle/ParticleThread.hxx"
#include "Render/Particle/ParticleSystem.hxx"


// Light & shadow
#include "Render/Light/ShadowMapFrustum.hxx"
#include "Render/Light/Light.hxx"
#include "Render/Light/LightSystem.hxx"


// Util
#include "Render/Util/LineRender.hxx"
#include "Render/Util/ScreenQuad.hxx"
#include "Render/Util/UnitSphere.hxx"

#include "Render/Material/Material.hxx"
#include "Render/Material/MaterialParameter.hxx"
#include "Render/Material/ShaderProgram.hxx"
#include "Render/Material/Texture.hxx"
#include "Render/Material/Technqiue.hxx"
#include "Render/Material/ParameterManager.hxx"
#include "Render/Material/SamplerState.hxx"


#include "Render/RenderSystem/RenderSetting.hxx"
#include "Render/RenderSystem/RenderContext.hxx"

#include "Render/RenderScheme/DeferredLightPass.hxx"
#include "Render/RenderScheme/DeferredShadowPass.hxx"
#include "Render/RenderScheme/GBufferPass.hxx"
#include "Render/RenderScheme/ShadowDepthPass.hxx"
#include "Render/RenderScheme/ShadingPass.hxx"
#include "Render/RenderScheme/RenderScheme.hxx"
#include "Render/RenderScheme/DefferredRender.hxx"

#include "Render/RenderScheme/BlurPostProcess.hxx"
#include "Render/RenderScheme/HDRPostProcess.hxx"

#include "Render/Terrain/Terrain.hxx"
#include "Render/Terrain/TerrainSection.hxx"


#include "Render/RTTI.h"

void RenderModuleInit()
{
	RenderSystemRTTIInit();

	RenderSetting*	pRenderSetting = new RenderSetting();
	SetRenderSetting(pRenderSetting);

	ParameterManager* pMaterialMang = new ParameterManager();
	SetParameterManager(pMaterialMang);

	RenderSystem* pRenderSystem = new RenderSystem();
	SetRenderSystem(pRenderSystem);

	ParticleSystem* pParticleMang = new ParticleSystem();
	SetParticleSystem(pParticleMang);

	Terrain* pTerrain = new Terrain();
	SetTerrain(pTerrain);

	LightSystem* pLightSystem = new LightSystem();
	SetLightSystem(pLightSystem);
}

void RenderModuleShutdown()
{
	Terrain* pTerrain = (Terrain*)GetTerrain();
	SAFE_DELETE(pTerrain);
	SetTerrain(NULL);

	RenderSystem* pRenderSystem = GetRenderSystem();
	SAFE_DELETE(pRenderSystem);
	SetRenderSystem(NULL);

	ParameterManager* pMaterialMang = GetParameterManager();
	SAFE_DELETE(pMaterialMang);
	SetParameterManager(NULL);

	RenderSystemRTTIShutdown();
}








