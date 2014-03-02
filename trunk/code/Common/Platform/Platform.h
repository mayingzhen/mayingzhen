#ifndef __Platform_H__
#define __Platform_H__


#if defined(_WIN32)
#define PLATFORM_WIN 1
#elif defined(__APPLE__)
#define PLAFTORM_IOS 1
#elif defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#endif






#if PLATFORM_WIN == 1
#include <Common/Platform/Win/PlatformWin.h>
#elif PLAFTORM_IOS == 1
#include <Common/Platform/iOS/PlatformiOS.h>
#elif PLATFORM_ANDROID == 1
#include <Common/Platform/Android/PlatformAndroid.h>
#endif

#define WINDOW_VSYNC        1


COMMON_API int DisplayError(const char * title, 
							const char * text, 
							const char * description, 
							const char * file, 
							int line);



COMMON_API void Tokenize(const std::string& str, std::vector<std::string>& tokens, 
			  const std::string& delimiters = " ", const bool trimEmpty = false);


#endif //__Platform_H__
