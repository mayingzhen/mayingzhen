#ifndef  _RenderSystem_RTTI__H__
#define  _RenderSystem_RTTI__H__

#include "Render/Terrain/TerrainSection.h"

using namespace ma;


#define RTTI_DECL(ClassType) \
	Object* Create_##ClassType(void* arg) { return new ClassType((GameObject*)arg);} \
	void Delete_##ClassType(Object* pObj) { SAFE_DELETE(pObj);}

RTTI_DECL(RenderMesh);
RTTI_DECL(ParticleEmitter);
RTTI_DECL(PointLight);
RTTI_DECL(SpotLight);
RTTI_DECL(DirectonalLight);
RTTI_DECL(Camera);
RTTI_DECL(TerrainSection);

#undef RTTI_DECL


// ResCreator
Resource*	MeshData_Creator() {return new MeshData();}
Resource*	Material_Creator() {return new Material();}
Resource*	TextureData_Creator() {return GetRenderDevice()->CreateTexture(NULL);}

void RenderSystemRTTIInit()
{
	RenderObject::StaticInitClass();
	Light::StaticInitClass();

#define RTTI_DECL(ClassType) \
	ClassType::StaticInitClass(); \
	ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType); \
	ObjectFactoryManager::GetInstance().RegisterObjectDeleteFactory(#ClassType,Delete_##ClassType);

	RTTI_DECL(RenderMesh);
	RTTI_DECL(ParticleEmitter);
	RTTI_DECL(PointLight);
	RTTI_DECL(SpotLight);
	RTTI_DECL(Camera);
	RTTI_DECL(DirectonalLight);
	RTTI_DECL(TerrainSection);

#undef RTTI_DECL

	// Resource
	MeshData::StaticInitClass();
	Material::StaticInitClass();
	Texture::StaticInitClass();

	ResourceSystem::RegisterResourceFactory("skn",MeshData_Creator);
	ResourceSystem::RegisterResourceFactory("mat",Material_Creator);
	ResourceSystem::RegisterResourceFactory("tga",TextureData_Creator);
	ResourceSystem::RegisterResourceFactory("jpg",TextureData_Creator);
	ResourceSystem::RegisterResourceFactory("png",TextureData_Creator);
	ResourceSystem::RegisterResourceFactory("dds",TextureData_Creator);

}

void RenderSystemRTTIShutdown()
{
	// Resource
	ResourceSystem::UnregisterResourceFactory("skn",MeshData_Creator);
	ResourceSystem::UnregisterResourceFactory("mat",Material_Creator);
	ResourceSystem::UnregisterResourceFactory("tga",TextureData_Creator);
	ResourceSystem::UnregisterResourceFactory("jpg",TextureData_Creator);
	ResourceSystem::UnregisterResourceFactory("png",TextureData_Creator);
	ResourceSystem::UnregisterResourceFactory("dds",TextureData_Creator);

	MeshData::StaticShutdownClass();
	Material::StaticShutdownClass();
	Texture::StaticShutdownClass();
}


#endif