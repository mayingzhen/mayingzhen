#ifndef  _GLESDeveiveContext__H__
#define  _GLESDeveiveContext__H__

#if PLATFORM_WIN == 1
#include "GLESRender/DeviceContext/GLESDeviceContextWindows.h"
#elif PLAFTORM_IOS == 1
#include "GLESDeviceContextIOS.h"
#elif PLATFORM_ANDROID == 1
#include "GLESRender/DeviceContext/GLESDeviceContextAndroid.h"
#endif


#endif
