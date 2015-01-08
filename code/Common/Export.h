#ifndef  _CommonExport__H__
#define  _CommonExport__H__

#ifdef _MSC_VER
// 	#ifdef COMMON_EXPORTS
// 		#define COMMON_API _declspec (dllexport)
// 	#else
// 		#define COMMON_API _declspec (dllimport)
// 	#endif
	#define  COMMON_API	
#else
	#define  COMMON_API	
#endif


#endif