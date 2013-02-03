#include <MonoScript/stdafx.h>
#include <MonoScript/MonoScriptModule.h>
#include <MonoScript/MonoScriptPlugin.h>


extern "C" MonoScriptAPI bool dllStartPlugin()
{
	MonoScriptModuleInit();

	return true;
}

extern "C" MonoScriptAPI const char* dllGetPluginName()
{
	return "ScriptPlugin";
}

extern "C" MonoScriptAPI bool dllStopPlugin()
{
	MonoScriptModuleShutdown();

	return true;
}
