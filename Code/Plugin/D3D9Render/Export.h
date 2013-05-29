#ifndef  _DxRenderExport__H__
#define  _DxRenderExport__H__

#ifdef D3D9RENDER_EXPORTS
#define D3D9RENDER_API _declspec (dllexport)
#else
#define D3D9RENDER_API _declspec (dllimport)
#endif


#endif