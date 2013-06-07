#ifndef __BaseType_h__
#define __BaseType_h__



typedef unsigned char		Uint8;
typedef unsigned short		Uint16;
typedef unsigned int		Uint;
typedef unsigned int		Uint32;
typedef char				Int8;
typedef short				Int16;
typedef unsigned char       Byte;
typedef unsigned short      WORD;
typedef unsigned long		DWORD;

#ifdef _WIN32
typedef __int64				Int64;
typedef unsigned __int64	Uint64;
#else
#include <stdint.h>
typedef int64_t				Int64;
typedef uint64_t			Uint64;
#endif


#ifdef  __cplusplus
extern "C" {
#endif

	/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifdef  __cplusplus
}
#endif




typedef unsigned short BoneIndex;



#endif //__BaseType_h__

