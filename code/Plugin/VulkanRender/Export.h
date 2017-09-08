#pragma once

#ifdef _MSC_VER
#	ifdef VULKANRENDER_EXPORTS
#		define VULKAN_API _declspec (dllexport)
#	else
#		define VULKAN_API _declspec (dllimport)
#	endif
#else
#	define VULKAN_API 
#endif
