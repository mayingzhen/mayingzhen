#ifndef  _Engine_RTTI__H__
#define  _Engine_RTTI__H__

#include "RenderSystem/RTTI.h"
#include "Physics/RTTI.h"
#include "Resource/RTTI.h"

using namespace ma;

// RTTI
Object*	Create_AnimationObject(void* arg) {return GetAnimationSystem()->CreateAnimationObject( (GameObject*)arg );}
void	Delete_AnimationObject(Object* pObj) {GetAnimationSystem()->DeleteAnimationObject((IAnimationObject*)pObj);}

Object*	Create_ScriptObject(void* arg) {return GetScriptSystem()->CreateScriptObject( (GameObject*)arg );}
void	Delete_ScriptObject(Object* pObj) {GetScriptSystem()->DeleteScriptObject( (IScriptObject*)pObj );}


void EngineRTTIInit()
{
	RenderSystemRTTIInit();

	IPhysicsSystemRTTIInit();

	ResourceSystemRTTIInit();

#define RTTI_DECL(ClassType,showType) \
	ClassType::StaticInitClass(); \
	ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##showType);  \
	ObjectFactoryManager::GetInstance().RegisterObjectDeleteFactory(#ClassType,Delete_##showType);

	RTTI_DECL(IAnimationObject,AnimationObject);
	RTTI_DECL(IScriptObject,ScriptObject);
#undef RTTI_DECL

}

void EngineRTTIShutdown()
{
	ResourceSystemRTTIShutdown();

	IPhysicsSystemRTTIShutdown();

	RenderSystemRTTIShutdown();

}

#endif