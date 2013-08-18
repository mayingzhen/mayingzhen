#ifndef _Thread_h__
#define _Thread_h__

#include "AutoLock.h"

#if PLATFORM_WIN == 1
#include "WinThread/CriticalSection_win.h"
#include "WinThread/Event_win.h"
#include "WinThread/Thread_win.h"
#elif PLAFTORM_IOS == 1 || PLATFORM_ANDROID == 1
#include "pthread/CriticalSection_Android.h"
#include "pthread/Event_Android.h"
#include "pthread/Thread_Android.h"
#endif



#endif // _Thread_h__