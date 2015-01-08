#ifndef  _Engine_RTTI__H__
#define  _Engine_RTTI__H__

//#include "Animation/RTTI.h"
#include "Script/RTTI.h"
#include "Physics/RTTI.h"
#include "Resource/RTTI.h"
#include "RenderSystem/RTTI.h"

namespace ma
{
	void EngineRTTIInit()
	{
		// RTII
		ClassManager* pClsMan = new ClassManager();
		SetClassManager(pClsMan);

		ObjectFactoryManager* pObjMan = new ObjectFactoryManager();
		SetObjectFactoryManager(pObjMan);

		ResourceSystem* pRsourceSystem = new ResourceSystem();
		SetResourceSystem(pRsourceSystem);
		pRsourceSystem->Init();

		Object::StaticInitClass();
		
		SceneNode::StaticInitClass();

		Context* pContexMan = new Context();
		SetContext(pContexMan);

		SceneNode::RegisterObject(pContexMan);

		Component::StaticInitClass();
		RenderComponent::StaticInitClass();
	
		IScriptSystemRTTIInit();

		IPhysicsSystemRTTIInit();

		//IAnimationSystemRTTIInit();

		ResourceSystemRTTIInit();

		RenderSystemRTTIInit();
	}

	void EngineRTTIShutdown()
	{
		RenderSystemRTTIShutdown();

		ResourceSystemRTTIShutdown();

		IPhysicsSystemRTTIShutdown();

		IScriptSystemRTTIShoutdown();

		//IAnimationSystemRTTIShoutdown();

		RenderComponent::StaticShutdownClass();
		Component::StaticShutdownClass();

		Object::StaticShutdownClass();

		ResourceSystem* pRsourceSystem = GetResourceSystem();
		pRsourceSystem->ShoutDown(); 
		SAFE_DELETE(pRsourceSystem);
		SetResourceSystem(NULL);

		delete GetObjectFactoryManager();
		
		delete GetClassManager();
	}
}



#endif