#ifndef _Thread_h__
#define _Thread_h__


#if PLATFORM_WIN == 1
#include "WinThread/WinEvent.h"
#include "WinThread/WinThread.h"
#else
#include "pthread/PosixEvent.h"
#include "pthread/PosixThread.h"
#endif



#endif // _Thread_h__