#ifndef PVR_H
#define PVR_H

#include "il_internal.h"

#ifdef _WIN32
#pragma pack(push, pvr_struct, 1)
#endif

typedef struct PVRHEAD
{
	ILuint headerLength;
	ILuint height;
	ILuint width;
	ILuint numMipmaps;
	ILuint flags;
	ILuint dataLength;
	ILuint bpp;
	ILuint bitmaskRed;
	ILuint bitmaskGreen;
	ILuint bitmaskBlue;
	ILuint bitmaskAlpha;
	ILuint pvrTag;
	ILuint numSuffs;
}IL_PACKSTRUCT PVRHEAD;

#ifdef _WIN32
#pragma pack(pop, pvr_struct)
#endif
ILboolean iLoadPvrInternal(ILvoid);
ILboolean SwapModuData(unsigned int* Data);
ILuint TwiddleUV(ILuint YSize, ILuint XSize, ILuint YPos, ILuint XPos);
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#endif
