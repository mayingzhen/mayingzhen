#ifndef  _RenderSystemExport__H__
#define  _RenderSystemExport__H__

#ifdef _MSC_VER
	#ifdef RENDER_EXPORTS
		#define RENDER_API _declspec (dllexport)
	#else
		#define RENDER_API _declspec (dllimport)
	#endif
#else
	#define  RENDER_API	
#endif


#endif