#pragma once

#ifdef _MSC_VER
#	ifdef METALRENDER_EXPORTS
#		define METALRENDER_API _declspec (dllexport)
#	else
#		define METALRENDER_API _declspec (dllimport)
#	endif
#else
#	define METALRENDER_API 
#endif
