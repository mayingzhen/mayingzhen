#ifndef  _Engine_RTTI__H__
#define  _Engine_RTTI__H__

#include "Animation/RTTI.h"
#include "Script/RTTI.h"
#include "Physics/RTTI.h"
#include "Resource/RTTI.h"

namespace ma
{
	void EngineRTTIInit()
	{
		IScriptSystemRTTIInit();

		IPhysicsSystemRTTIInit();

		IAnimationSystemRTTIInit();

		ResourceSystemRTTIInit();
	}

	void EngineRTTIShutdown()
	{
		ResourceSystemRTTIShutdown();

		IPhysicsSystemRTTIShutdown();

		IScriptSystemRTTIShoutdown();

		IAnimationSystemRTTIShoutdown();
	}
}



#endif