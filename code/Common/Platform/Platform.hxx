#include "Platform.h"


#if PLATFORM_WIN == 1
#include "Win/PlatformWin.hxx"
#elif PLAFTORM_IOS == 1
#include "Ios/PlatformIos.hxx"
#elif PLATFORM_ANDROID == 1
#include "Android/PlatformAndroid.hxx"
#endif







