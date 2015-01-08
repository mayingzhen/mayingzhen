#ifndef __BaseType_h__
#define __BaseType_h__


#include <string>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <fstream>
#include <stdarg.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <assert.h>
#include <limits>
using namespace std;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;
typedef char				int8;
typedef short				int16;
typedef unsigned char       Byte;
typedef unsigned short      WORD;
typedef unsigned long		DWORD;
typedef unsigned short		BoneIndex;
typedef float				Real;




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








#endif //__BaseType_h__

