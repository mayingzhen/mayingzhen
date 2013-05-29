#ifndef  _FramworkExport__H__
#define  _FramworkExport__H__

#ifdef _MSC_VER

	#ifdef FRAMWORK_EXPORTS
	#define FRAMWORK_API _declspec (dllexport)
	#else
	#define FRAMWORK_API _declspec (dllimport)
	#endif

#else

	#define FRAMWORK_API

#endif

#endif