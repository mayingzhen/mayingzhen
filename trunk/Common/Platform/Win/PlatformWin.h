#ifndef __PlatformWin_H__
#define __PlatformWin_H__

#include <WinSock.h>
#include <windows.h>


COMMON_API int gettimeofday(struct timeval * val, struct timezone *);



#endif //__PlatformWin_H__