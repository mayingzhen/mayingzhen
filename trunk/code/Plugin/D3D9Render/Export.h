#ifndef  _D3D9RenderExport__H__
#define  _D3D9RenderExport__H__

#ifdef _MSC_VER

	#ifdef D3D9RENDER_EXPORTS
	#define D3D9RENDER_API _declspec (dllexport)
	#else
	#define D3D9RENDER_API _declspec (dllimport)
	#endif

#else

	#define D3D9RENDER_API

#endif


#endif