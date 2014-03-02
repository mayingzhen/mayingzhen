#ifndef  _FbxImportExport__H__
#define  _FbxImportExport__H__


#ifdef FBXIMPORT_EXPORTS
#define FBXIMPORT_API _declspec (dllexport)
#else
#define FBXIMPORT_API _declspec (dllimport)
#endif


#endif
