#ifndef  _CommonExport__H__
#define  _CommonExport__H__

#ifdef COMMON_EXPORTS
#define COMMON_API _declspec (dllexport)
#else
#define COMMON_API _declspec (dllimport)
#endif


#endif