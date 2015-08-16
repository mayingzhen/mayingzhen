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


void RenderSystemRTTIInit()
{
	MeshComponent::RegisterAttribute();
	SkinMeshComponent::RegisterAttribute();
	Camera::RegisterAttribute();
	Terrain::RegisterAttribute();
	RenderState::RegisterAttribute();

	GetResourceSystem()->RegisterResourceFactory("skn",MeshData_Creator);
}

void RenderSystemRTTIShutdown()
{
	GetResourceSystem()->UnregisterResourceFactory("skn",MeshData_Creator);
}


#endif