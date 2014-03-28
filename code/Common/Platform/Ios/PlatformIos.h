#ifndef __PlatformIOS_h__
#define __PlatformIOS_h__

#include <sys/time.h>
#include <time.h>
#include <unistd.h>

typedef unsigned long ULONG;
typedef unsigned int  UINT;
typedef unsigned int  UINT32;
typedef unsigned int  UINT_PTR;
typedef unsigned char byte;
typedef void* HWND;
typedef void* HINSTANCE;
typedef long HRESULT;
#define _stricmp strcasecmp
#define _strnicmp strncasecmp

#define MAX_PATH 1024

#define S_OK     ((HRESULT)0L)
#define S_FALSE  ((HRESULT)1L)


void _OutputDebugString(const char* buf);

#endif //__PlatformAndroid_h__