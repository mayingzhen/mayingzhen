#include "il_atc.h"
#define DDS_LINEARSIZE 0x00080000L

#ifndef IL_NO_ATC

ILboolean iGetAtcHeader(ATC_HEADER* pHeader)
{
	ILint i;
	iread(&pHeader->Signature, 1, 4);
	pHeader->Size1 = GetLittleUInt();
	pHeader->Flags1 = GetLittleUInt();
	pHeader->Height = GetLittleUInt();
	pHeader->Width = GetLittleUInt();
	pHeader->LinearSize = GetLittleUInt();
	pHeader->Depth = GetLittleUInt();
	pHeader->MipMapCount = GetLittleUInt();
	pHeader->AlphaBitDepth = GetLittleUInt();

	for(i = 0; i < 10; ++i)
	{
		pHeader->NotUsed[i] = GetLittleUInt();
	}

	pHeader->Size2 = GetLittleUInt();
	pHeader->Flags2 = GetLittleUInt();

	iread(&pHeader->FourCC, 1, 4);

	pHeader->RGBBitCount = GetLittleUInt();
	pHeader->RBitMask = GetLittleUInt();
	pHeader->GBitMask = GetLittleUInt();
	pHeader->BBitMask = GetLittleUInt();
	pHeader->RGBAlphaBitMask = GetLittleUInt();
	pHeader->ddsCaps1 = GetLittleUInt();
	pHeader->ddsCaps2 = GetLittleUInt();
	pHeader->ddsCaps3 = GetLittleUInt();
	pHeader->ddsCaps4 = GetLittleUInt();
	pHeader->TextureStage = GetLittleUInt();

	if(pHeader->Depth == 0)
	{
		pHeader->Depth = 1;
	}
	return IL_TRUE;
}

ILboolean ilLoadAtcF(ILHANDLE File)
{
	ILuint FirstPos;
	ILboolean bRet;

	iSetInputFile(File);
	FirstPos = itell();

	bRet = LoadAtcInternal();
	
	iseek(FirstPos, IL_SEEK_SET);
	return bRet;
}

ILboolean ilLoadAtcL(const ILvoid* Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return LoadAtcInternal();
}

ILboolean ilLoadAtc(const ILstring FileName)
{
	ILHANDLE AtcFile;
	ILboolean bAtc = IL_FALSE;

	AtcFile = iopenr(FileName);
	if(AtcFile == NULL)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bAtc;
	}

	bAtc = ilLoadAtcF(AtcFile);
	icloser(AtcFile);
 
	return bAtc;
}

ILboolean LoadAtcInternal()
{
	ILuint nWidth = 0;
	ILuint nHeight = 0;
	ILuint nMipLevel = 0;
	ILuint nDataSize = 0; 
	ILuint format;
	ILimage* Image = NULL;
	ILimage* StartMipMap = NULL;
	ATC_HEADER header;

	if(iCurImage == NULL)
	{
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if(!iGetAtcHeader(&header))
	{
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	Image = iCurImage;

	nWidth = header.Width;
	nHeight = header.Height;

	if(header.FourCC == ' CTA')
	{
		format = IL_ATC_FORMAT;
	}
	else if(header.FourCC == 'ICTA')
	{
		format = IL_ATCI_FORMAT;
	}
	else if(header.FourCC == 'ACTA')
	{
		format = IL_ATCA_FORMAT;
	}
	else
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	
	while(nMipLevel < header.MipMapCount) 
	{
		if(nMipLevel > 0)
		{
			Image->Next = (ILimage*)ialloc(sizeof(ILimage));
			Image = Image->Next;

			if(nMipLevel == 1)
			{
				StartMipMap = Image;
			}
		}

		if(nMipLevel == 0)
		{
			if(header.Flags1 & DDS_LINEARSIZE)
			{
				nDataSize = header.LinearSize;		
			}
			else
			{
				nDataSize = (nWidth * header.RGBBitCount / 8) * nHeight * header.Depth;
			}
		}
		else
		{
			nDataSize = IL_MAX(nDataSize / 4, (format==IL_ATC_FORMAT? 8 : 16));
		}		
		
		Image->Width = nWidth;
		Image->Height = nHeight;
		Image->Depth = 1;
		Image->SizeOfData = nDataSize;
		Image->Data = (ILubyte*)ialloc(nDataSize);
		Image->Pal.PalType = IL_PAL_NONE;
		Image->Layers = NULL;
		Image->AnimList = NULL;
		Image->Profile = NULL;
		Image->DxtcData = NULL;
		Image->Next = NULL;
		Image->Mipmaps = NULL;
		Image->Format = format;

		if(iread(Image->Data, 1, nDataSize) != nDataSize)
		{
			ifree(Image->Data);
			Image->Data = NULL;
			return IL_FALSE;
		}

		nWidth = IL_MAX(nWidth >> 1, 1);
		nHeight = IL_MAX(nHeight >> 1, 1);
		nMipLevel++;
	}

	iCurImage->Mipmaps = StartMipMap;
	iCurImage->Next = NULL;
	ilBindImage(ilGetCurName());
	nDataSize = 0;
	return IL_TRUE;
}

#endif//IL_NO_ATC