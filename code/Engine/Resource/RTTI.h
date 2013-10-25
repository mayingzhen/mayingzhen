#ifndef  _ResoureceSystem_RTTI__H__
#define  _ResoureceSystem_RTTI__H__


using namespace ma;


// ResCreator
Resource*	MeshData_Creator() {return new MeshData();}
Resource*	Material_Creator() {return new Material();}
Resource*	TextureData_Creator() {return GetRenderDevice()->CreateTexture(NULL);}

void ResourceSystemRTTIInit()
{
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

void ResourceSystemRTTIShutdown()
{
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