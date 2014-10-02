#ifndef  _Engine_RTTI__H__
#define  _Engine_RTTI__H__

//#include "Animation/RTTI.h"
#include "Script/RTTI.h"
#include "Physics/RTTI.h"
#include "Resource/RTTI.h"

namespace ma
{
	void EngineRTTIInit()
	{
		// RTII
		ClassManager* pClsMan = new ClassManager();
		ObjectFactoryManager* pObjMan = new ObjectFactoryManager();
		Object::StaticInitClass();

		Component::StaticInitClass();
		RenderComponent::StaticInitClass();
	
		IScriptSystemRTTIInit();

		IPhysicsSystemRTTIInit();

		//IAnimationSystemRTTIInit();

		ResourceSystemRTTIInit();
	}

	void EngineRTTIShutdown()
	{
		ResourceSystemRTTIShutdown();

		IPhysicsSystemRTTIShutdown();

		IScriptSystemRTTIShoutdown();

		//IAnimationSystemRTTIShoutdown();

		RenderComponent::StaticShutdownClass();
		Component::StaticShutdownClass();

		Object::StaticShutdownClass();
		ObjectFactoryManager::GetInstance().Shutdown();
		delete ObjectFactoryManager::GetInstancePtr();
		ClassManager::GetInstance().Shutdown();
		delete ClassManager::GetInstancePtr();
	}
}



#endif