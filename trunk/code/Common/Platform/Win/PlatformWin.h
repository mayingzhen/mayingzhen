#ifndef __PlatformWin_H__
#define __PlatformWin_H__

#include <WinSock.h>
#include <windows.h>

#define snprintf _snprintf

COMMON_API int gettimeofday(struct timeval * val, struct timezone *);

COMMON_API void _OutputDebugString(const char* buf);

#endif //__PlatformWin_H__