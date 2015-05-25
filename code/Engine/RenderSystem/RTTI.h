#ifndef  _RenderSystem_RTTI__H__
#define  _RenderSystem_RTTI__H__

#include "Engine/Scene/MeshComponent.h"
#include "Engine/Scene/Particle/ParticleEmitter.h"
#include "Engine/Scene/Light/Light.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/Terrain/Terrain.h"
#include "Engine/Renderable/MeshData.h"
#include "Engine/Material/Material.h"
#include "Engine/Material/Texture.h"
#include "Engine/RenderSystem/IRenderDevice/IRenderDevice.h"


using namespace ma;


#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();} 
#include "RTTIDecl.h"
#undef RTTI_DECL


// ResCreator
Resource*	MeshData_Creator() {return new MeshData();}
Resource*	Material_Creator() {return new Material();}
Resource*	SamplerState_Creator() {return GetRenderDevice()->CreateTexture();}
Resource*	ShaderProgram_Creator() {return GetRenderDevice()->CreateShaderProgram();}

void RenderSystemRTTIInit()
{
	Light::StaticInitClass();
	Terrain::StaticInitClass();
	Texture::StaticInitClass();
	ShaderProgram::StaticInitClass();
	
#define RTTI_DECL(ClassType) \
	ClassType::StaticInitClass(); \
	GetObjectFactoryManager()->RegisterObjectFactory(#ClassType,Create_##ClassType); 
	#include "RTTIDecl.h"
#undef RTTI_DECL

	MeshComponent::RegisterAttribute();
	SkinMeshComponent::RegisterAttribute();
	Camera::RegisterAttribute();
	Terrain::RegisterAttribute();
	Texture::RegisterAttribute();
	ShaderProgram::RegisterAttribute();


	GetResourceSystem()->RegisterResourceFactory("skn",MeshData_Creator);
	GetResourceSystem()->RegisterResourceFactory("mat",Material_Creator);
// 	GetResourceSystem()->RegisterResourceFactory("tga",ImageData_Creator);
// 	GetResourceSystem()->RegisterResourceFactory("jpg",ImageData_Creator);
// 	GetResourceSystem()->RegisterResourceFactory("png",ImageData_Creator);
// 	GetResourceSystem()->RegisterResourceFactory("dds",ImageData_Creator);
// 	GetResourceSystem()->RegisterResourceFactory("pvr",ImageData_Creator);
// 	GetResourceSystem()->RegisterResourceFactory("pkm",ImageData_Creator);
	GetResourceSystem()->RegisterResourceFactory("shader",ShaderProgram_Creator);
	GetResourceSystem()->RegisterResourceFactory("sampler",SamplerState_Creator);

}

void RenderSystemRTTIShutdown()
{
	GetResourceSystem()->UnregisterResourceFactory("skn",MeshData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("mat",Material_Creator);
// 	GetResourceSystem()->UnregisterResourceFactory("tga",ImageData_Creator);
// 	GetResourceSystem()->UnregisterResourceFactory("jpg",ImageData_Creator);
// 	GetResourceSystem()->UnregisterResourceFactory("png",ImageData_Creator);
// 	GetResourceSystem()->UnregisterResourceFactory("dds",ImageData_Creator);
// 	GetResourceSystem()->UnregisterResourceFactory("pvr",ImageData_Creator);
// 	GetResourceSystem()->UnregisterResourceFactory("pkm",ImageData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("tech",ShaderProgram_Creator);
	GetResourceSystem()->UnregisterResourceFactory("sampler",SamplerState_Creator);

#define RTTI_DECL(ClassType) \
	ClassType::StaticShutdownClass(); \
	GetObjectFactoryManager()->UnRegisterObjectFactory(#ClassType,Create_##ClassType); 
	#include "RTTIDecl.h"
#undef RTTI_DECL

	Light::StaticShutdownClass();
	Terrain::StaticShutdownClass();
	Texture::StaticShutdownClass();
	ShaderProgram::StaticShutdownClass();
}


#endif