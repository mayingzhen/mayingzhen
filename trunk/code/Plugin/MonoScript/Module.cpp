#include <MonoScript/stdafx.h>
#include "Module.h"

#include "ScriptObject.hxx"
#include "ScriptSystem.hxx"
#include "IntenalCall.hxx"
#include "ClassField.hxx"
#include "ScriptClass.hxx"


using namespace ma;

void MonoScriptModuleInit()
{
	ScriptSystem* pScriptDevive = new ScriptSystem();
	pScriptDevive->Init();
	SetScriptSystem(pScriptDevive);
}

void MonoScriptModuleShutdown()
{
	ScriptSystem* pScriptDevive = (ScriptSystem*)GetScriptSystem();
	pScriptDevive->Shutdown();
	delete pScriptDevive;
	SetScriptSystem(NULL);
}

#if PLATFORM_WIN == 1
extern "C" MonoScriptAPI bool dllStartPlugin()
{
	MonoScriptModuleInit();

	return true;
}


extern "C" MonoScriptAPI bool dllStopPlugin()
{
	MonoScriptModuleShutdown();

	return true;
}
#endif
