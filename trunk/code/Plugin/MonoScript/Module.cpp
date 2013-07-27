#include <MonoScript/stdafx.h>
#include "Module.h"

#include "ScriptObject.hxx"
#include "ScriptDevice.hxx"
#include "IntenalCall.hxx"
#include "ClassField.hxx"
#include "ScriptClass.hxx"


using namespace ma;

void MonoScriptModuleInit()
{
	ScriptDevice* pScriptDevive = new ScriptDevice();
	pScriptDevive->Init();
	SetScriptDevice(pScriptDevive);
}

void MonoScriptModuleShutdown()
{
	ScriptDevice* pScriptDevive = (ScriptDevice*)GetScriptDevice();
	pScriptDevive->Shutdown();
	delete pScriptDevive;
	SetScriptDevice(NULL);
}

