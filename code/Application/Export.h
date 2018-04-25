#pragma once

#ifdef _MSC_VER
	#ifdef APP_EXPORTS
		#define APP_API _declspec (dllexport)
	#else
		#define APP_API _declspec (dllimport)
	#endif
#else
	#define  APP_API	
#endif

