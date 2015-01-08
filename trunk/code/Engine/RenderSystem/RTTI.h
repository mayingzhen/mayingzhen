#ifndef  _RenderSystem_RTTI__H__
#define  _RenderSystem_RTTI__H__

#include "Engine/Scene/MeshComponent.h"
#include "Engine/Scene/Particle/ParticleEmitter.h"
#include "Engine/Scene/Light/Light.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/Terrain/Terrain.h"
#include "Engine/RenderScheme/RenderPass.h"
#include "Engine/RenderScheme/ShadingPass.h"
#include "Engine/RenderScheme/GBufferPass.h"
#include "Engine/RenderScheme/DeferredLightPass.h"
#include "Engine/RenderScheme/ShadowDepthPass.h"
#include "Engine/RenderScheme/DeferredShadowPass.h"
#include "Engine/Renderable/MeshData.h"
#include "Engine/Material/MaterialData.h"
#include "Engine/Material/Texture.h"
#include "Engine/RenderSystem/IRenderDevice/IRenderDevice.h"


using namespace ma;


#define RTTI_DECL(ClassType) \
	Object* Create_##ClassType(void* arg) { return new ClassType((SceneNode*)arg);} \
	void Delete_##ClassType(Object* pObj) { /*SAFE_DELETE(pObj);*/}
RTTI_DECL(MeshComponent);
RTTI_DECL(ParticleEmitter);
RTTI_DECL(PointLight);
RTTI_DECL(SpotLight);
RTTI_DECL(DirectonalLight);
RTTI_DECL(Camera);
//RTTI_DECL(TerrainSection);
#undef RTTI_DECL


#define RTTI_DECL(ClassType) \
	Object* Create_##ClassType() { return new ClassType();} \
	void Delete_##ClassType(Object* pObj) { /*SAFE_DELETE(pObj)*/;}
RTTI_DECL(ShadowDepthPass);
RTTI_DECL(GBufferPass);
RTTI_DECL(DeferredLightPass);
RTTI_DECL(DeferredShadowPass);
RTTI_DECL(ShadingPass);
#undef RTTI_DECL


// ResCreator
Resource*	MeshData_Creator() {return new MeshData();}
Resource*	MaterialData_Creator() {return new MaterialData();}
Resource*	TextureData_Creator() {return GetRenderDevice()->CreateTexture(NULL);}
Resource*	ShaderProgram_Creator() {return GetRenderDevice()->CreateShaderProgram();}


void RenderSystemRTTIInit()
{

	Light::StaticInitClass();

	RenderPass::StaticInitClass();

#define RTTI_DECL(ClassType) \
	ClassType::StaticInitClass(); \
	GetObjectFactoryManager()->RegisterObjectFactory(#ClassType,Create_##ClassType); 

	RTTI_DECL(MeshComponent);
	RTTI_DECL(ParticleEmitter);
	RTTI_DECL(PointLight);
	RTTI_DECL(SpotLight);
	RTTI_DECL(Camera);
	RTTI_DECL(DirectonalLight);
	//RTTI_DECL(TerrainSection);

	RTTI_DECL(ShadowDepthPass);
	RTTI_DECL(GBufferPass);
	RTTI_DECL(DeferredLightPass);
	RTTI_DECL(DeferredShadowPass);
	RTTI_DECL(ShadingPass);

#undef RTTI_DECL
	
	MaterialParameter::StaticInitClass();
	Terrain::StaticInitClass();

	MeshComponent::RegisterObject( GetContext() );
	Camera::RegisterObject( GetContext() );
	Terrain::RegisterObject( GetContext() );
	//MaterialParameter::RegisterObject( GetContext() );

	MeshData::StaticInitClass();
	MaterialData::StaticInitClass();
	Texture::StaticInitClass();
	ShaderProgram::StaticInitClass();

	GetResourceSystem()->RegisterResourceFactory("skn",MeshData_Creator);
	GetResourceSystem()->RegisterResourceFactory("mat",MaterialData_Creator);
	GetResourceSystem()->RegisterResourceFactory("tga",TextureData_Creator);
	GetResourceSystem()->RegisterResourceFactory("jpg",TextureData_Creator);
	GetResourceSystem()->RegisterResourceFactory("png",TextureData_Creator);
	GetResourceSystem()->RegisterResourceFactory("dds",TextureData_Creator);
	GetResourceSystem()->RegisterResourceFactory("pvr",TextureData_Creator);
	GetResourceSystem()->RegisterResourceFactory("pkm",TextureData_Creator);
	GetResourceSystem()->RegisterResourceFactory("tech",ShaderProgram_Creator);

}

void RenderSystemRTTIShutdown()
{
	GetResourceSystem()->UnregisterResourceFactory("skn",MeshData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("mat",MaterialData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("tga",TextureData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("jpg",TextureData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("png",TextureData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("tech",ShaderProgram_Creator);

	MeshData::StaticShutdownClass();
	MaterialData::StaticShutdownClass();
	Texture::StaticShutdownClass();
	ShaderProgram::StaticShutdownClass();

	Light::StaticShutdownClass();

	RenderPass::StaticShutdownClass();

#define RTTI_DECL(ClassType) \
	ClassType::StaticShutdownClass(); \
	GetObjectFactoryManager()->UnRegisterObjectFactory(#ClassType,Create_##ClassType); \
	
	RTTI_DECL(MeshComponent);
	RTTI_DECL(ParticleEmitter);
	RTTI_DECL(PointLight);
	RTTI_DECL(SpotLight);
	RTTI_DECL(Camera);
	RTTI_DECL(DirectonalLight);
	//RTTI_DECL(TerrainSection);

	RTTI_DECL(ShadowDepthPass);
	RTTI_DECL(GBufferPass);
	RTTI_DECL(DeferredLightPass);
	RTTI_DECL(DeferredShadowPass);
	RTTI_DECL(ShadingPass);

#undef RTTI_DECL


}


#endif