#include <MonoScript/stdafx.h>
#include <MonoScript/MonoScriptModule.h>


#include <MonoScript/MonoScriptResource.hxx>
#include <MonoScript/MonoScriptObject.hxx>
#include <MonoScript/MonoScriptDevice.hxx>
#include <MonoScript/MonoScriptInternal.hxx>

using namespace ma;

void ScriptModuleInit()
{
	MonoScriptDevice* pScriptDevive = new MonoScriptDevice();
	pScriptDevive->Init();
	SetScriptDevice(pScriptDevive);
}

void ScriptModuleShutdown()
{
	MonoScriptDevice* pScriptDevive = (MonoScriptDevice*)GetScriptDevice();
	pScriptDevive->Shutdown();
	delete pScriptDevive;
	SetScriptDevice(NULL);
}

