#ifndef  _GLESRender_Export__H__
#define  _GLESRender_Export__H__

#ifdef _MSC_VER
#	ifdef GLESRENDER_EXPORTS
#		define GLESRENDER_API _declspec (dllexport)
#	else
#		define GLESRENDER_API _declspec (dllimport)
#	endif
#else
#	define GLESRENDER_API 
#endif

#endif