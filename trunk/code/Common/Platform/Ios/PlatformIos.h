#ifndef __PlatformAndroid_h__
#define __PlatformAndroid_h__

#include <sys/time.h>
#include <time.h>

typedef unsigned long ULONG;
typedef unsigned int  UINT;
typedef unsigned int  UINT32;
typedef void* HWND;
typedef void* HINSTANCE;
#define _stricmp strcasecmp
#define _strnicmp strncasecmp

#define MAX_PATH 1024


void OutputDebugString(const char* buf);

#endif //__PlatformAndroid_h__