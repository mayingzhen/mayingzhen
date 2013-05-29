#ifndef __PlatformAndroid_h__
#define __PlatformAndroid_h__


typedef unsigned long ULONG;
typedef unsigned int  UINT;
typedef unsigned int  UINT32;
typedef void* HWND;
#define _stricmp strcasecmp
#define _strnicmp strncasecmp


void OutputDebugString(const char* buf);

#endif //__PlatformAndroid_h__