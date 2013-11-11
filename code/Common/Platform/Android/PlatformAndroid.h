#ifndef __PlatformAndroid_h__
#define __PlatformAndroid_h__


typedef unsigned long ULONG;
typedef unsigned int  UINT;
typedef unsigned int  UINT32;
typedef unsigned int  UINT_PTR;
typedef unsigned char byte;
typedef void* HWND;
typedef long HRESULT;
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define MAX_PATH 256

#define S_OK     ((HRESULT)0L)
#define S_FALSE  ((HRESULT)1L)

void _OutputDebugString(const char* buf);

#endif //__PlatformAndroid_h__