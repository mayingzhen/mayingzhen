#ifndef  _RenderSystem_RTTI__H__
#define  _RenderSystem_RTTI__H__

#include "Engine/Terrain/TerrainSection.h"

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

}

void RenderSystemRTTIShutdown()
{

}


#endif