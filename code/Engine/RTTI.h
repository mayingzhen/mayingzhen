#ifndef  _Engine_RTTI__H__
#define  _Engine_RTTI__H__

#include "RenderSystem/RTTI.h"

namespace ma
{
	void EngineRTTIInit()
	{
		Object::StaticInitClass();
		
		SceneNode::StaticInitClass();

		SceneNode::RegisterAttribute();

		Component::StaticInitClass();
		RenderComponent::StaticInitClass();

		Resource::StaticInitClass();

		RenderSystemRTTIInit();
	}

	void EngineRTTIShutdown()
	{
		RenderSystemRTTIShutdown();

		Resource::StaticShutdownClass();

		RenderComponent::StaticShutdownClass();
		Component::StaticShutdownClass();

		Object::StaticShutdownClass();
	}
}



#endif