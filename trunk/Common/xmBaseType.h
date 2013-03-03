#ifndef __xmBaseType_h__
#define __xmBaseType_h__


typedef unsigned char		xmUint8;
typedef unsigned short		xmUint16;
typedef unsigned int		UINT;
typedef unsigned int		xmUint32;

typedef char				xmInt8;
typedef short				xmInt16;
typedef int					xmInt;
typedef int					xmInt32;


#ifdef _WIN32
typedef __int64				xmInt64;
typedef unsigned __int64	xmUint64;
#else
typedef int64_t				xmInt64
typedef unsigned int64_t	xmUInt64;
#endif

typedef unsigned char       xmByte;
typedef unsigned short      xmWord;



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



#endif //__xmBaseType_h__

