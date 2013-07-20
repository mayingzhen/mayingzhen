/*
*  il_etc.h
*  DevIL
*
*  Created by shenqiliang on 10-10-26.
*  Copyright 2010 TqDigital. All rights reserved.
*
*/

#ifndef KTX_H
#define KTX_H

#include "il_internal.h"

#ifdef _WIN32
#pragma pack(push, ktx_struct, 1)
#endif


typedef struct KTX_HEADER
{
	ILubyte identifier[12];
	ILuint endianness;
	ILuint glType;
	ILuint glTypeSize;
	ILuint glFormat;
	ILuint glInternalFormat;
	ILuint glBaseInternalFormat;
	ILuint pixelWidth;
	ILuint pixelHeight;
	ILuint pixelDepth;
	ILuint numberOfArrayElements;
	ILuint numberOfFaces;
	ILuint numberOfMipmapLevels;
	ILuint bytesOfKeyValueData;
} KTX_HEADER;

#ifdef _WIN32
#pragma pack(pop,ktx_struct)
#endif

ILboolean ilLoadKtx(const ILstring FileName);
ILboolean LoadKtxInternal(ILvoid);
ILboolean LoadKtxInternalDecompress(ILvoid);
ILvoid decompressBlockDiffFlip(ILuint block_part1, ILuint block_part2, ILubyte *img,ILint width,ILint height,ILint startx,ILint starty);
ILvoid read_big_endian_4byte_word(ILuint *blockadr);
#endif
