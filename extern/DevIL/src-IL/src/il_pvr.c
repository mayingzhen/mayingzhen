#include "il_pvr.h"

#ifndef IL_NO_PVR

#define PVR_TEXTURE_FLAG_TYPE_MASK 0xff
#define PVR_ADD_BORDER_FLAG 0x00000800
PVRHEAD PvrHeader;
enum 
{
	kPVRTextureFlagTypePVRTC_2 = 24,
	kPVRTextureFlagTypePVRTC_4 = 25,
	kPVRTextureFlagTypePVRTC_DX_2 = 12,
	kPVRTextureFlagTypePVRTC_DX_4 = 13,
	kPVRTexturePixelTypeRGBA_4444 = 16,
};

const unsigned int PVRTC2_MIN_TEXWIDTH		= 16;
const unsigned int PVRTC2_MIN_TEXHEIGHT		= 8;
const unsigned int PVRTC4_MIN_TEXWIDTH		= 8;
const unsigned int PVRTC4_MIN_TEXHEIGHT		= 8;

typedef struct
{
	/* Uses 64 bits pre block*/
	unsigned int PackedData[2];
}AMTC_BLOCK_STRUCT;

ILboolean iGetPvrHeader(PVRHEAD* header)
{
	header->headerLength = GetLittleUInt();
	header->height = GetLittleUInt();
	header->width = GetLittleUInt();
	header->numMipmaps = GetLittleUInt();
	header->flags = GetLittleUInt();
	header->dataLength = GetLittleUInt();
	header->bpp = GetLittleUInt();
	header->bitmaskRed = GetLittleUInt();
	header->bitmaskGreen = GetLittleUInt();
	header->bitmaskBlue = GetLittleUInt();
	header->bitmaskAlpha = GetLittleUInt();
	header->pvrTag = GetLittleUInt();
	header->numSuffs = GetLittleUInt();

	return IL_TRUE;
}

ILboolean ilLoadPvr(const ILstring FileName)
{
	ILHANDLE PvrFile;
	ILboolean bPvr = IL_FALSE;
	
	PvrFile = iopenr(FileName);
	if (PvrFile == NULL) 
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPvr;
	}
	
	bPvr = ilLoadPvrF(PvrFile);
	icloser(PvrFile);
	
	return bPvr;
}

ILboolean ilLoadPvrF(ILHANDLE File)
{
	ILuint FirstPos;
	ILboolean bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadPvrInternal();
	iseek(FirstPos, IL_SEEK_SET);
		return bRet;
}

ILboolean ilLoadPvrL(const ILvoid* Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadPvrInternal();
}

ILuint TwiddleUV(ILuint YSize, ILuint XSize, ILuint YPos, ILuint XPos)
{
	ILuint Twiddled;
	
	ILuint MinDimension;
	ILuint MaxValue;
	
	ILuint SrcBitPos;
	ILuint DstBitPos;
	
	int ShiftCount;
	
	if(YSize < XSize)
	{
		MinDimension = YSize;
		MaxValue	 = XPos;
	}
	else
	{
		MinDimension = XSize;
		MaxValue	 = YPos;
	}
		
	/*
	 // Step through all the bits in the "minimum" dimension
	 */
	SrcBitPos = 1;
	DstBitPos = 1;
	Twiddled  = 0;
	ShiftCount = 0;
	
	while(SrcBitPos < MinDimension)
	{
		if(YPos & SrcBitPos)
		{
			Twiddled |= DstBitPos;
		}
		
		if(XPos & SrcBitPos)
		{
			Twiddled |= (DstBitPos << 1);
		}
		
		
		SrcBitPos <<= 1;
		DstBitPos <<= 2;
		ShiftCount += 1;
		
	}/*end while*/
	
	/*
	 // prepend any unused bits
	 */
	MaxValue >>= ShiftCount;
	
	Twiddled |=  (MaxValue << (2*ShiftCount));
	
	return Twiddled;
}

ILboolean iLoadPvrInternal(ILvoid)
{
	ILuint formatFlags = 0;
	ILuint dataLength = 0;
	ILboolean bHasAlpha = IL_FALSE;
	ILuint dataSize = 0;
	ILuint heightBlocks = 0;
	ILuint dataOffset = 0;
	ILuint width = 0;
	ILuint height = 0;
	ILimage* Image = NULL;
	ILimage* StartMipMap = NULL;
	ILuint mipLevel = 0;
	
	if(iCurImage == NULL)
	{
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	
	//header
	if(!iGetPvrHeader(&PvrHeader))
	{
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}	
	
	formatFlags = PvrHeader.flags & PVR_TEXTURE_FLAG_TYPE_MASK;
		
	Image = iCurImage;
	width = PvrHeader.width;
	height = PvrHeader.height;
	dataLength = PvrHeader.dataLength;
	bHasAlpha = PvrHeader.bitmaskAlpha;
	
	while (dataOffset < dataLength) 
	{
		if (mipLevel> 0)
		{
			Image->Next = (ILimage*)ialloc(sizeof(ILimage));
			Image = Image->Next;
		
			if(mipLevel == 1)
			{
				StartMipMap = Image;
			}
		}
	
		if (formatFlags == kPVRTextureFlagTypePVRTC_4 || 
			formatFlags == kPVRTextureFlagTypePVRTC_DX_4) 
		{
			Image->Format = bHasAlpha ? IL_PVRTCRGBA4  : IL_PVRTCRGB4;
			dataSize = ( IL_MAX(width, PVRTC4_MIN_TEXWIDTH) * IL_MAX(height, PVRTC4_MIN_TEXHEIGHT) * 4 + 7) / 8;
			
		}
		else if(formatFlags == kPVRTextureFlagTypePVRTC_2 || 
				formatFlags == kPVRTextureFlagTypePVRTC_DX_2)
		{
			Image->Format = bHasAlpha ? IL_PVRTCRGBA2 : IL_PVRTCRGB2;
			dataSize = ( IL_MAX(width, PVRTC2_MIN_TEXWIDTH) * IL_MAX(height, PVRTC2_MIN_TEXHEIGHT) * 2 + 7) / 8;
		}
		else if(formatFlags == kPVRTexturePixelTypeRGBA_4444)
		{
			Image->Format = IL_PVR_RGBA4444;
			dataSize = width * height * 2;
		}
		else 
		{
			ilSetError(IL_OUT_FORMAT_SAME);
			return IL_FALSE;
		}
		
		Image->Width = width;
		Image->Height = height;
		Image->Depth = 1;
		Image->Data = (ILubyte*)ialloc(dataSize);
		Image->SizeOfData = dataSize;
		Image->Pal.PalType = IL_PAL_NONE;
		Image->Layers = NULL;
		Image->AnimList = NULL;
		Image->Profile = NULL;
		Image->DxtcData = NULL;
		Image->Next = NULL;
		Image->Mipmaps = NULL;
		Image->PvrAddBorder = IL_FALSE;

		if(PvrHeader.flags & PVR_ADD_BORDER_FLAG)
		{
			Image->PvrAddBorder = IL_TRUE;
		}
		
		if(iread(Image->Data, 1, dataSize) != dataSize)
		{
			ifree(Image->Data);
			Image->Data = NULL;
			return IL_FALSE;
		}
		
		dataOffset += dataSize;
		width = MAX(width >> 1, 1);
		height = MAX(height >> 1, 1);
		mipLevel++;
	}
	
	iCurImage->Mipmaps = StartMipMap;
	iCurImage->Next = NULL;
	ilBindImage(ilGetCurName());
	return IL_TRUE;
}

ILboolean SwapModuData(unsigned int* Data)
{
	unsigned int T00Mask = 3;
	unsigned int T01Mask = T00Mask<<2;
	unsigned int T02Mask = T01Mask<<2;
	unsigned int T03Mask = T02Mask<<2;
	unsigned int T10Mask = T03Mask<<2;
	unsigned int T11Mask = T10Mask<<2;
	unsigned int T12Mask = T11Mask<<2;
	unsigned int T13Mask = T12Mask<<2;
	unsigned int T20Mask = T13Mask<<2;
	unsigned int T21Mask = T20Mask<<2;
	unsigned int T22Mask = T21Mask<<2;
	unsigned int T23Mask = T22Mask<<2;
	unsigned int T30Mask = T23Mask<<2;
	unsigned int T31Mask = T30Mask<<2;
	unsigned int T32Mask = T31Mask<<2;
	unsigned int T33Mask = T32Mask<<2;
	
	unsigned int T00 = (*Data) & T00Mask;
	unsigned int T01 = (*Data) & T01Mask;
	unsigned int T02 = (*Data) & T02Mask;
	unsigned int T03 = (*Data) & T03Mask;
	unsigned int T10 = (*Data) & T10Mask;
	unsigned int T11 = (*Data) & T11Mask;
	unsigned int T12 = (*Data) & T12Mask;
	unsigned int T13 = (*Data) & T13Mask;
	unsigned int T20 = (*Data) & T20Mask;
	unsigned int T21 = (*Data) & T21Mask;
	unsigned int T22 = (*Data) & T22Mask;
	unsigned int T23 = (*Data) & T23Mask;
	unsigned int T30 = (*Data) & T30Mask;
	unsigned int T31 = (*Data) & T31Mask;
	unsigned int T32 = (*Data) & T32Mask;
	unsigned int T33 = (*Data) & T33Mask;
	
	
    T00=T00<<24;
	T30=T30>>24;

	T01=T01<<24;
	T31=T31>>24;

	T02=T02<<24;
	T32=T32>>24;

	T03=T03<<24;
	T33=T33>>24;
	
	T10=T10<<8;
	T20=T20>>8;

	T11=T11<<8;
	T21=T21>>8;

	T12=T12<<8;
	T22=T22>>8;

	T13=T13<<8;
	T23=T23>>8;	
	
	(*Data) = (T00 | T01 | T02 | T03
		 | T10 | T11 | T12 | T13
	     | T20 | T21 | T22 | T23
	     | T30 | T31 | T32 | T33);
			
	return IL_TRUE;
}

#endif//IL_NO_PVR