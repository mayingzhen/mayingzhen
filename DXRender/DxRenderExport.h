#ifndef  _DxRenderExport__H__
#define  _DxRenderExport__H__

#ifdef DXRENDER_EXPORTS
#define DXRENDER_API _declspec (dllexport)
#else
#define DXRENDER_API _declspec (dllimport)
#endif


#endif