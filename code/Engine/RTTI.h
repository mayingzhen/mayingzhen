#ifndef  _Engine_RTTI__H__
#define  _Engine_RTTI__H__

#include "RenderSystem/RTTI.h"

namespace ma
{
	void EngineRTTIInit()
	{
		SceneNode::RegisterAttribute();

		RenderSystemRTTIInit();
	}

	void EngineRTTIShutdown()
	{
		RenderSystemRTTIShutdown();
	}
}



#endif