#include "Thread.h"


#if PLATFORM_WIN == 1
#include "WinThread/WinThread.hxx"
#else
#include "pthread/PosixThread.hxx"
#endif
