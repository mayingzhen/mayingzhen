#ifndef  _Engine_RTTI__H__
#define  _Engine_RTTI__H__


using namespace ma;

// RTTI
#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <Engine/RTTIDecl.h>
#undef RTTI_DECL

Resource* MeshData_Creator() {return new MeshData();}
Resource* Material_Creator() {return new Material();}
Resource* TextureData_Creator() {return GetRenderDevice()->CreateTexture(NULL);}

Object*		Texture_Creator() {return GetRenderDevice()->CreateTexture(NULL);}

void EngineRTTIInit()
{

#define RTTI_DECL(ClassType) ClassType::StaticInitClass();
#include <Engine/RTTIDecl.h>
#undef RTTI_DECL

#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType);
#include <Engine/RTTIDecl.h>
#undef RTTI_DECL

	Texture::StaticInitClass();
	ObjectFactoryManager::GetInstance().RegisterObjectFactory("Texture",Texture_Creator);

	ResourceSystem::RegisterResourceFactory("skn",MeshData_Creator);
	ResourceSystem::RegisterResourceFactory("mat",Material_Creator);
	ResourceSystem::RegisterResourceFactory("tga",TextureData_Creator);
	ResourceSystem::RegisterResourceFactory("jpg",TextureData_Creator);
	ResourceSystem::RegisterResourceFactory("png",TextureData_Creator);
	ResourceSystem::RegisterResourceFactory("dds",TextureData_Creator);

}

void EngineRTTIShutdown()
{
	ResourceSystem::UnregisterResourceFactory("skn",MeshData_Creator);
	ResourceSystem::UnregisterResourceFactory("mat",Material_Creator);
	ResourceSystem::UnregisterResourceFactory("tga",TextureData_Creator);
	ResourceSystem::UnregisterResourceFactory("jpg",TextureData_Creator);
	ResourceSystem::UnregisterResourceFactory("png",TextureData_Creator);
	ResourceSystem::UnregisterResourceFactory("dds",TextureData_Creator);

	Texture::StaticShutdownClass();
	ObjectFactoryManager::GetInstance().UnRegisterObjectFactory("Texture",Texture_Creator);

#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().UnRegisterObjectFactory(#ClassType,Create_##ClassType);
#include <Engine/RTTIDecl.h>
#undef RTTI_DECL

#define RTTI_DECL(ClassType) ClassType::StaticShutdownClass();
#include <Engine/RTTIDecl.h>
#undef RTTI_DECL


}

#endif