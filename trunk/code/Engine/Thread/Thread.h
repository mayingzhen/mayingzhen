#ifndef _Thread_h__
#define _Thread_h__

#include "AutoLock.h"

#if PLATFORM_WIN == 1
#include "Win/CriticalSection_win.h"
#include "Win/Event_win.h"
#include "Win/Thread_win.h"
#elif PLAFTORM_IOS == 1
#include "IOS/CriticalSection_ios.h"
#include "IOS/Event_IOS.h"
#include "IOS/Thread_IOS.h"
#elif PLATFORM_ANDROID == 1
#include "Android/CriticalSection_Android.h"
#include "Android/Event_Android.h"
#include "Android/Thread_Android.h"
#endif



#endif // _Thread_h__