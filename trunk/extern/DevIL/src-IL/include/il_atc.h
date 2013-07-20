#ifndef ATC_H
#define ATC_H

#include "il_internal.h"

#ifdef _WIN32
#pragma pack(push, atc_struct, 1)
#endif

typedef struct ATC_HEADER
{
	ILbyte Signature[4];

	ILuint Size1;			// size of the structure (minus MagicNum)
	ILuint Flags1;			// determines what fields are valid
	ILuint Height;			// height of surface to be created
	ILuint Width;			// width of input surface
	ILuint LinearSize;		// Formless late-allocated optimized surface size
	ILuint Depth;			// Depth if a volume texture
	ILuint MipMapCount;		// number of mip-map levels requested
	ILuint AlphaBitDepth;	// depth of alpha buffer requested
	ILuint NotUsed[10];

	ILuint Size2;			// size of structure
	ILuint Flags2;			// pixel format flags
	ILuint FourCC;			// (FOURCC code)
	ILuint RGBBitCount;		// how many bits per pixel
	ILuint RBitMask;		// mask for red bit
	ILuint GBitMask;		// mask for green bits
	ILuint BBitMask;		// mask for blue bits
	ILuint RGBAlphaBitMask; // mask for alpha channel
	ILuint ddsCaps1, ddsCaps2, ddsCaps3, ddsCaps4;	// direct draw surface capabilities
	ILuint TextureStage;
} IL_PACKSTRUCT ATC_HEADER;

#ifdef _WIN32
#pragma pack(pop,atc_struct)
#endif

ILboolean ilLoadAtc(const ILstring FileName);
ILboolean LoadAtcInternal(ILvoid);

#endif//ATC_H