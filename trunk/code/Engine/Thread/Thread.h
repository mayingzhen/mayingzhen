#ifndef _Thread_h__
#define _Thread_h__

#include "AutoLock.h"

#if PLATFORM_WIN == 1
#include "WinThread/WinCriticalSection.h"
#include "WinThread/WinEvent.h"
#include "WinThread/WinThread.h"
#else
#include "pthread/PosixCriticalSection.h"
#include "pthread/PosixEvent.h"
#include "pthread/PosixThread.h"
#endif



#endif // _Thread_h__