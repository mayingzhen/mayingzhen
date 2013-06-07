#ifndef __MonoScriptPlugin_H__
#define __MonoScriptPlugin_H__


extern "C" MonoScriptAPI bool dllStartPlugin();

extern "C" MonoScriptAPI const char* dllGetPluginName();

extern "C" MonoScriptAPI bool dllStopPlugin();


#endif //__MonoScriptPlugin_H__
