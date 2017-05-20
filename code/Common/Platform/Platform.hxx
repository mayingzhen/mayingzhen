#include "Platform.h"


#if PLATFORM_WIN == 1
#include <Common/Platform/Win/PlatformWin.hxx>
#elif PLAFTORM_IOS == 1
#include <Common/Platform/Ios/PlatformIos.hxx>
#elif PLATFORM_ANDROID == 1
#include <Common/Platform/Android/PlatformAndroid.hxx>
#endif







