#ifndef  _ResourceRegister__H__
#define  _ResourceRegister__H__


using namespace ma;


// ResCreator
Resource*	MeshData_Creator() {return new MeshData();}
Resource*	MaterialData_Creator() {return new MaterialData();}
Resource*	TextureData_Creator() {return GetRenderDevice()->CreateTexture(NULL);}

void ResourceRegisterInit()
{
	MeshData::StaticInitClass();
	MaterialData::StaticInitClass();
	Texture::StaticInitClass();

	ResourceSystem::RegisterResourceFactory("skn",MeshData_Creator);
	ResourceSystem::RegisterResourceFactory("mat",MaterialData_Creator);
	ResourceSystem::RegisterResourceFactory("tga",TextureData_Creator);
	ResourceSystem::RegisterResourceFactory("jpg",TextureData_Creator);
	ResourceSystem::RegisterResourceFactory("png",TextureData_Creator);
	ResourceSystem::RegisterResourceFactory("dds",TextureData_Creator);

}

void ResourceRegisterShutdown()
{
	ResourceSystem::UnregisterResourceFactory("skn",MeshData_Creator);
	ResourceSystem::UnregisterResourceFactory("mat",MaterialData_Creator);
	ResourceSystem::UnregisterResourceFactory("tga",TextureData_Creator);
	ResourceSystem::UnregisterResourceFactory("jpg",TextureData_Creator);
	ResourceSystem::UnregisterResourceFactory("png",TextureData_Creator);
	ResourceSystem::UnregisterResourceFactory("dds",TextureData_Creator);

	MeshData::StaticShutdownClass();
	MaterialData::StaticShutdownClass();
	Texture::StaticShutdownClass();
}


#endif