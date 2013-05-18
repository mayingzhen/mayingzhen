#include "Platform.h"

#if PLATFORM_WIN == 1
#include "Win/PlatformWin.hxx"
#elif PLAFTORM_IOS == 1
#elif PLATFORM_ANDROID == 1
#include "Android/PlatformAndroid.h"
#endif
