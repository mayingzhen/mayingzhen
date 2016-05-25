#include "Wwise/stdafx.h"
#include "Wwise/Modelue.h"

#include "IO/AkFileLocationBase.hxx"
#include "IO/AkDefaultIOHookBlocking.hxx"
#include "AllocHook.hxx"
#include "WwiseBank.hxx"
#include "WwiseComponent.hxx"
#include "WwiseListener.hxx"
#include "WwiseSystem.hxx"



#define DEMO_DEFAULT_POOL_SIZE 2*1024*1024
#define DEMO_LENGINE_DEFAULT_POOL_SIZE 2*1024*1024

namespace ma
{
	void WWiseModuleInit()
	{
		g_pWwiseBankManger = new ResourceSystem<WwiseBank>();
		g_pWwiseSystem = new WwiseSystem();	
	}

	void WWiseModuleShutdown()
	{
		SAFE_DELETE(g_pWwiseSystem);
		SAFE_DELETE(g_pWwiseBankManger);
	}

}

#if PLATFORM_WIN == 1
extern "C" bool dllStartPlugin()
{
	ma::WWiseModuleInit();

	return true;
}


extern "C" bool dllStopPlugin()
{
	ma::WWiseModuleShutdown();

	return true;
}
#endif

