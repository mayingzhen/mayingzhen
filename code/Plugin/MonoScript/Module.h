#ifndef __MonoScriptModule_H__
#define __MonoScriptModule_H__

#include <mono/metadata/metadata.h>
#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/tokentype.h>



#include <MonoScript/ScriptObject.h>
#include <MonoScript/ScriptSystem.h>

void MonoScriptModuleInit();

void MonoScriptModuleShutdown();


#endif // __ScriptModule_H__
