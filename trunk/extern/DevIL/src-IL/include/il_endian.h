//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 01/29/2002 <--Y2K Compliant! =]
//
// Filename: src-IL/include/il_endian.h
//
// Description: Handles Endian-ness
//
//-----------------------------------------------------------------------------
#ifndef IL_ENDIAN_H
#define IL_ENDIAN_H

#include "il_internal.h"

#ifdef WORDS_BIGENDIAN		//this is defined by ./configure
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__
#endif
#endif
#ifdef __BIG_ENDIAN__
#define Short(s)	iSwapShort(s)
#define UShort(s)	iSwapUShort(s)
#define Int(i)		iSwapInt(i)
#define UInt(i)		iSwapUInt(i)
#define Float(f)	iSwapFloat(f)
#define Double(d)	iSwapDouble(d)
#define BigShort(s)
#define BigUShort(s)
#define BigInt(i)
#define BigUInt(i)
#define BigFloat(f)
#define BigDouble(d)
#else
#undef __LITTLE_ENDIAN__	// Not sure if it's defined by any compiler...
#define __LITTLE_ENDIAN__
#define Short(s)
#define UShort(s)
#define Int(i)
#define UInt(i)
#define Float(f)
#define Double(d)
#define BigShort(s)		iSwapShort(s)
#define BigUShort(s)	iSwapUShort(s)
#define BigInt(i)		iSwapInt(i)
#define BigUInt(i)		iSwapUInt(i)
#define BigFloat(f)		iSwapFloat(f)
#define BigDouble(d)	iSwapDouble(d)
#endif
ILvoid iSwapUShort(ILushort* s);
ILvoid iSwapShort(ILshort* s);
ILvoid iSwapUInt(ILuint* i);
ILvoid iSwapInt(ILint* i);
ILvoid iSwapFloat(ILfloat* f);
ILvoid iSwapDouble(ILdouble* d);
ILushort GetLittleUShort();
ILshort GetLittleShort();
ILuint GetLittleUInt();
ILint GetLittleInt();
ILfloat GetLittleFloat();
ILdouble GetLittleDouble();
ILushort GetBigUShort();
ILshort GetBigShort();
ILuint GetBigUInt();
ILint GetBigInt();
ILfloat GetBigFloat();
ILdouble GetBigDouble();
ILubyte SaveLittleUShort(ILushort s);
ILubyte SaveLittleShort(ILshort s);
ILubyte SaveLittleUInt(ILuint i);
ILubyte SaveLittleInt(ILint i);
ILubyte SaveLittleFloat(ILfloat f);
ILubyte SaveLittleDouble(ILdouble d);
ILubyte SaveBigUShort(ILushort s);
ILubyte SaveBigShort(ILshort s);
ILubyte SaveBigUInt(ILuint i);
ILubyte SaveBigInt(ILint i);
ILubyte SaveBigFloat(ILfloat f);
ILubyte SaveBigDouble(ILdouble d);

#ifdef IL_ENDIAN_C
#undef INLINE
#define INLINE inline
#endif

ILvoid EndianSwapData(void* _Image);
#endif //ENDIAN_H
