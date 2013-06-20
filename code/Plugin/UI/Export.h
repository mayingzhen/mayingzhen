#ifndef  _UI_Export__H__
#define  _UI_Export__H__

#ifdef _MSC_VER
	#ifdef UI_EXPORTS
	#define UI_API _declspec (dllexport)
	#else
	#define UI_API _declspec (dllimport)
	#endif
#else
	#define UI_API 
#endif

#endif