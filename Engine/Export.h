#ifndef  _ApplicationExport__H__
#define  _ApplicationExport__H__

#ifdef ENGINE_EXPORTS
#define ENGINE_API _declspec (dllexport)
#else
#define ENGINE_API _declspec (dllimport)
#endif


#endif