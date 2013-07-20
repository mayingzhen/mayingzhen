#ifndef  _EengineExport__H__
#define  _EengineExport__H__

#ifdef _MSC_VER

	#ifdef ENGINE_EXPORTS
	#define ENGINE_API _declspec (dllexport)
	#else
	#define ENGINE_API _declspec (dllimport)
	#endif

#else

	#define ENGINE_API

#endif

#endif