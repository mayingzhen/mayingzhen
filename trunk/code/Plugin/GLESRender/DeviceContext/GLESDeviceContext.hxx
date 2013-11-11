#if PLATFORM_WIN == 1
#include "GLESDeviceContextWindows.hxx"
#elif PLAFTORM_IOS == 1
//#include "GLESDeviceContextIOS.mm"
#elif PLATFORM_ANDROID == 1
#include "GLESDeviceContextAndroid.hxx"
#endif
