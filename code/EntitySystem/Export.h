#ifndef  _EntitySystemExport__H__
#define  _EntitySystemExport__H__

#ifdef _MSC_VER

	#ifdef EntitySystem_EXPORTS
	#define ENTITYSYSTEM_API _declspec (dllexport)
	#else
	#define ENTITYSYSTEM_API _declspec (dllimport)
	#endif

#else

	#define ENTITYSYSTEM_API

#endif // _MSC_VER

#endif // _EntitySystemExport__H__