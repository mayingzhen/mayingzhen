#include <MonoScript/stdafx.h>
#include "Module.h"

#include "ScriptObject.hxx"
#include "ScriptSystem.hxx"
#include "IntenalCall.hxx"
#include "ClassField.hxx"
#include "ScriptClass.hxx"


using namespace ma;

RefPtr<Object> Create_ScriptObject() { return new ScriptObject();}

void MonoScriptModuleInit()
{
	ScriptSystem* pScriptDevive = new ScriptSystem();
	SetScriptSystem(pScriptDevive);

	GetObjectFactoryManager()->RegisterObjectFactory("ScriptObject",Create_ScriptObject);
}

void MonoScriptModuleShutdown()
{
	GetObjectFactoryManager()->UnRegisterObjectFactory("ScriptObject",Create_ScriptObject);

	ScriptSystem* pScriptDevive = (ScriptSystem*)GetScriptSystem();
	delete pScriptDevive;
	SetScriptSystem(NULL);
}

#if defined(_WIN32)
extern "C" bool dllStartPlugin()
{
	MonoScriptModuleInit();

	return true;
}


extern "C" bool dllStopPlugin()
{
	MonoScriptModuleShutdown();

	return true;
}
#endif
