#ifndef  _RenderSystemExport__H__
#define  _RenderSystemExport__H__

#ifdef _MSC_VER
#	ifdef RENDERSYSTEM_EXPORTS
#		define RENDERSYSTEM_API _declspec (dllexport)
#	else
#		define RENDERSYSTEM_API _declspec (dllimport)
#	endif
#else 
#	define RENDERSYSTEM_API
#endif


#endif // _RenderSchemeExport__H__