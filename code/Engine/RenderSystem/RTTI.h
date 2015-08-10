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



//ResCreator
Resource*	MeshData_Creator() {return new MeshData();}
Resource*	Material_Creator() {return new Material();}
Resource*	SamplerState_Creator() {return GetRenderDevice()->CreateTexture();}
Resource*	ShaderProgram_Creator() {return GetRenderDevice()->CreateShaderProgram();}


void RenderSystemRTTIInit()
{
	MeshComponent::RegisterAttribute();
	SkinMeshComponent::RegisterAttribute();
	Camera::RegisterAttribute();
	Terrain::RegisterAttribute();
	Texture::RegisterAttribute();
	ShaderProgram::RegisterAttribute();

	GetResourceSystem()->RegisterResourceFactory("skn",MeshData_Creator);
	GetResourceSystem()->RegisterResourceFactory("mat",Material_Creator);
	GetResourceSystem()->RegisterResourceFactory("shader",ShaderProgram_Creator);
	GetResourceSystem()->RegisterResourceFactory("sampler",SamplerState_Creator);

}

void RenderSystemRTTIShutdown()
{
	GetResourceSystem()->UnregisterResourceFactory("skn",MeshData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("mat",Material_Creator);
	GetResourceSystem()->UnregisterResourceFactory("tech",ShaderProgram_Creator);
	GetResourceSystem()->UnregisterResourceFactory("sampler",SamplerState_Creator);
}


#endif