#ifndef  _RenderSystem_RTTI__H__
#define  _RenderSystem_RTTI__H__

//#include "Render/Terrain/TerrainSection.h"
#include "ResourceRTTI.h"

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


void RenderSystemRTTIInit()
{
	Light::StaticInitClass();

	RenderPass::StaticInitClass();

#define RTTI_DECL(ClassType) \
	ClassType::StaticInitClass(); \
	ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType); \
	ObjectFactoryManager::GetInstance().RegisterObjectDeleteFactory(#ClassType,Delete_##ClassType);

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

	ResourceRegisterInit();

}

void RenderSystemRTTIShutdown()
{
	ResourceRegisterShutdown();


}


#endif