#ifndef  _ApplicationExport__H__
#define  _ApplicationExport__H__

#ifdef APPLICATION_EXPORTS
#define Application_API _declspec (dllexport)
#else
#define Application_API _declspec (dllimport)
#endif


#endif