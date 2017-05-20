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
#include <Common/Platform/Ios/PlatformIos.h>
#elif PLATFORM_ANDROID == 1
#include <Common/Platform/Android/PlatformAndroid.h>
#endif

#define WINDOW_VSYNC        1




#endif //__Platform_H__
