#ifndef  _RenderSchemeExport__H__
#define  _RenderSchemeExport__H__

#ifdef _MSC_VER
#	ifdef RENDERSCHEME_EXPORTS
#		define RENDERSCHEME_API _declspec (dllexport)
#	else
#		define RENDERSCHEME_API _declspec (dllimport)
#	endif
#else 
#	define RENDERSCHEME_API
#endif


#endif // _RenderSchemeExport__H__