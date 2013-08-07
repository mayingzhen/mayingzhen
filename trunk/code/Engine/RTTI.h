#ifndef  _Engine_RTTI__H__
#define  _Engine_RTTI__H__


using namespace ma;

// RTTI
#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <Engine/RTTIDecl.h>
#undef RTTI_DECL

Resource* MeshData_Creator() {return new MeshData();}
Resource* TextureData_Creator() {return GetRenderDevice()->CreateRendTexture();}

Object*		Texture_Creator() {return GetRenderDevice()->CreateRendTexture();}

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

	ResourceManager::RegisterResourceFactory("skn",MeshData_Creator);
	ResourceManager::RegisterResourceFactory("tga",TextureData_Creator);
	ResourceManager::RegisterResourceFactory("jpg",TextureData_Creator);
	ResourceManager::RegisterResourceFactory("png",TextureData_Creator);
	ResourceManager::RegisterResourceFactory("dds",TextureData_Creator);

}

void EngineRTTIShutdown()
{
	ResourceManager::UnregisterResourceFactory("skn",MeshData_Creator);
	ResourceManager::UnregisterResourceFactory("tga",TextureData_Creator);
	ResourceManager::UnregisterResourceFactory("jpg",TextureData_Creator);
	ResourceManager::UnregisterResourceFactory("png",TextureData_Creator);
	ResourceManager::UnregisterResourceFactory("dds",TextureData_Creator);

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