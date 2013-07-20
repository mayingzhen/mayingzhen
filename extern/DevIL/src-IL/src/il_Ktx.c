
//ktx format spec : http://www.khronos.org/opengles/sdk/tools/KTX/file_format_spec/

#include "il_Ktx.h"

#ifndef IL_NO_KTX

enum {GL_ETC1_RGB8_OES=0x8d64};
static ILint unscramble[4] = {2, 3, 1, 0};
static ILint compressParams[16][4] = 
{{-8, -2,  2, 8}, {-8, -2,  2, 8}, 
{-17, -5, 5, 17}, {-17, -5, 5, 17},
{-29, -9, 9, 29}, {-29, -9, 9, 29},
{-42, -13, 13, 42}, {-42, -13, 13, 42},
{-60, -18, 18, 60}, {-60, -18, 18, 60},
{-80, -24, 24, 80}, {-80, -24, 24, 80},
{-106, -33, 33, 106}, {-106, -33, 33, 106},
{-183, -47, 47, 183}, {-183, -47, 47, 183}};

#define CLAMP(ll,x,ul) (((x)<(ll)) ? (ll) : (((x)>(ul)) ? (ul) : (x)))
#define GETBITS(source, size, startpos)  (( (source) >> ((startpos)-(size)+1) ) & ((1<<(size)) -1))
#define GETBITSHIGH(source, size, startpos)  (( (source) >> (((startpos)-32)-(size)+1) ) & ((1<<(size)) -1))
#define RED(img,width,x,y)   img[3*(y*width+x)+0]
#define GREEN(img,width,x,y) img[3*(y*width+x)+1]
#define BLUE(img,width,x,y)  img[3*(y*width+x)+2]
ILboolean bDecompressEtc = IL_FALSE;

ILboolean iGetKtxHeader(KTX_HEADER* header)
{
	iread(header->identifier, 1, 12);
	header->endianness = GetLittleUInt();
	header->glType = GetLittleUInt();
	header->glTypeSize = GetLittleUInt();
	header->glFormat= GetLittleUInt();
	header->glInternalFormat = GetLittleUInt();
	header->glBaseInternalFormat = GetLittleUInt();
	header->pixelWidth = GetLittleUInt();
	header->pixelHeight = GetLittleUInt();
	header->pixelDepth = GetLittleUInt();
	header->numberOfArrayElements = GetLittleUInt();
	header->numberOfFaces = GetLittleUInt();
	header->numberOfMipmapLevels = GetLittleUInt();
	header->bytesOfKeyValueData = GetLittleUInt();
	return IL_TRUE;
}
 
ILboolean ilLoadKtxL(const ILvoid* Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	if(bDecompressEtc == IL_FALSE)
	{
		return LoadKtxInternal();
	}
	else
	{
		return LoadKtxInternalDecompress();
	}
}

ILboolean ilLoadKtxF(ILHANDLE File)
{
	ILuint FirstPos;
	ILboolean bRet;
	
	iSetInputFile(File);
	FirstPos = itell();

	if(bDecompressEtc == IL_FALSE)
	{
		bRet = LoadKtxInternal();
	}
	else
	{
		bRet = LoadKtxInternalDecompress();
	}

	iseek(FirstPos, IL_SEEK_SET);
	return bRet;
}

ILboolean ilLoadKtx(const ILstring FileName)
{
	ILHANDLE KtxFile;
	ILboolean bKtx = IL_FALSE;
	
	KtxFile = iopenr(FileName);
	if(KtxFile == NULL)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bKtx;
	}
	
	bKtx = ilLoadKtxF(KtxFile);
	icloser(KtxFile);
	
	return bKtx;
}

ILboolean LoadKtxInternalDecompress()
{
	ILuint nWidth = 0;
	ILuint nHeight = 0;
	ILuint format = 0;
	ILuint nMipLevel = 0;
	ILuint nDataSize = 0;
	ILuint nMipPaddSize = 0;
	ILimage* Image = NULL;
	ILimage* StartMipMap = NULL;
	ILuint nBlockPart1 = 0;
	ILuint nBlockPart2 = 0;
	ILuint x = 0;
	ILuint y = 0;
	ILuint nActiveWidth = 0;
	ILuint nActiveHeight = 0;
	KTX_HEADER header;
	ILubyte* pTempDecompressData = NULL;
	
	if(iCurImage == NULL)
	{
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	
	if(!iGetKtxHeader(&header))
	{
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	iseek(header.bytesOfKeyValueData, IL_SEEK_CUR);

	Image = iCurImage;
	nActiveWidth = header.pixelWidth;
	nActiveHeight = header.pixelHeight;
	nWidth = ((nActiveWidth+3)/4)*4;
	nHeight = ((nActiveHeight+3)/4)*4;

	while (nMipLevel < header.numberOfMipmapLevels) 
	{
		if(nMipLevel> 0)
		{
			Image->Next = (ILimage*)ialloc(sizeof(ILimage));
			Image = Image->Next;

			if(nMipLevel == 1)
			{
				StartMipMap = Image;
			}
		}
		iseek(sizeof(ILuint), IL_SEEK_CUR);
		nDataSize = nActiveWidth * nActiveHeight * 3;
		Image->Width = nActiveWidth;
		Image->Height = nActiveHeight;
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
		Image->Format = IL_RGB;
		
		if(nHeight == nActiveHeight && nWidth == nActiveWidth)
		{
			pTempDecompressData = Image->Data;
		}
		else
		{
			pTempDecompressData = (ILubyte*)ialloc(nWidth * nHeight * 3);
		}
		
		for(y = 0; y < nHeight/4; y++)
		{
			for(x = 0; x < nWidth/4; x++)
			{
				read_big_endian_4byte_word(&nBlockPart1);
				read_big_endian_4byte_word(&nBlockPart2);
				decompressBlockDiffFlip(nBlockPart1, nBlockPart2, pTempDecompressData, nWidth, nHeight, 4*x, 4*y);
			}
		}

		//mipPadding
		nMipPaddSize = 3 - ((Image->SizeOfData + 3) % 4);
		iseek(nMipPaddSize, IL_SEEK_CUR);
		
		if(!(nHeight == nActiveHeight && nWidth == nActiveWidth))
		{
			// Convert from total area to active area:
			for(y = 0; y < nActiveHeight; y++)
			{
				for(x = 0; x < nActiveWidth; x++)
				{
					Image->Data[ (y*nActiveWidth)*3 + x*3 + 0 ] = pTempDecompressData[ (y*nWidth)*3 + x*3 + 0];
					Image->Data[ (y*nActiveWidth)*3 + x*3 + 1 ] = pTempDecompressData[ (y*nWidth)*3 + x*3 + 1];
					Image->Data[ (y*nActiveWidth)*3 + x*3 + 2 ] = pTempDecompressData[ (y*nWidth)*3 + x*3 + 2];
				}
			}
			free(pTempDecompressData);
		}
		nActiveHeight = IL_MAX(nActiveHeight >> 1, 1);
		nActiveWidth = IL_MAX(nActiveWidth >> 1, 1);
		nWidth = ((nActiveWidth+3)/4)*4;
		nHeight = ((nActiveHeight+3)/4)*4;
		nMipLevel++;
	}

	iCurImage->Mipmaps = StartMipMap;
	iCurImage->Next = NULL;
	ilBindImage(ilGetCurName());
	return IL_TRUE;
}

ILboolean LoadKtxInternal()
{
	ILuint nWidth = 0;
	ILuint nHeight = 0;
	ILuint nActiveWidth = 0;
	ILuint nActiveHeight = 0;
	ILuint nMipLevel = 0;
	ILuint nDataSize = 0;
	ILimage* Image = NULL;
	ILimage* StartMipMap = NULL;
	KTX_HEADER header;
	ILubyte* pKeyValueData = NULL;
	ILubyte* pMipPadding = NULL;
	ILuint nMipPaddSize = 0;

	if(iCurImage == NULL)
	{
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
		
	if(!iGetKtxHeader(&header))
	{
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}
	
	iseek(header.bytesOfKeyValueData, IL_SEEK_CUR);

	Image = iCurImage;
	nActiveWidth = header.pixelWidth;
	nActiveHeight = header.pixelHeight;
	nWidth = ((nActiveWidth+3)/4)*4;
	nHeight = ((nActiveHeight+3)/4)*4;
		
	while (nMipLevel < header.numberOfMipmapLevels) 
	{
		if (nMipLevel> 0)
		{
			Image->Next = (ILimage*)ialloc(sizeof(ILimage));
			Image = Image->Next;
			
			if(nMipLevel == 1)
			{
				StartMipMap = Image;
			}
		}

		//nDataSize = (nWidth/4) * (nHeight/4) * 2 * sizeof(ILuint);		
		Image->Width = nActiveWidth;
		Image->Height = nActiveHeight;
		Image->Depth = 1;
		iread(&nDataSize, 1, sizeof(ILuint));
		Image->SizeOfData = nDataSize;
		Image->Data = (ILubyte*)ialloc(nDataSize);
		Image->Pal.PalType = IL_PAL_NONE;
		Image->Layers = NULL;
		Image->AnimList = NULL;
		Image->Profile = NULL;
		Image->DxtcData = NULL;
		Image->Next = NULL;
		Image->Mipmaps = NULL;
		Image->Format = IL_ETC_FORMAT;

		if(iread(Image->Data, 1, nDataSize) != nDataSize)
		{
			ifree(Image->Data);
			Image->Data = NULL;
			return IL_FALSE;
		}
		
		// MipPadding offset
		nMipPaddSize = 3 - ((Image->SizeOfData + 3) % 4);
		iseek(nMipPaddSize, IL_SEEK_CUR);
	
		nActiveHeight = IL_MAX(nActiveHeight >> 1, 1);
		nActiveWidth = IL_MAX(nActiveWidth >> 1, 1);
		nWidth = ((nActiveWidth+3)/4)*4;
		nHeight = ((nActiveHeight+3)/4)*4;
		nMipLevel++;
	}
	
	iCurImage->Mipmaps = StartMipMap;
	iCurImage->Next = NULL;
	ilBindImage(ilGetCurName());
	nDataSize = 0;
	return IL_TRUE;
}

void decompressBlockDiffFlip(ILuint block_part1, ILuint block_part2, ILubyte *img,ILint width,ILint height,ILint startx,ILint starty)
{
	ILubyte avg_color[3], enc_color1[3], enc_color2[3];
	ILbyte diff[3];
	ILint table;
	ILint index,shift;
	ILint r,g,b;
	ILint diffbit;
	ILint flipbit;
	ILuint pixel_indices_MSB, pixel_indices_LSB;
	ILint x,y;

	diffbit = (GETBITSHIGH(block_part1, 1, 33));
	flipbit = (GETBITSHIGH(block_part1, 1, 32));

	if( !diffbit )
	{

		// We have diffbit = 0.

		// First decode left part of block.
		avg_color[0]= GETBITSHIGH(block_part1, 4, 63);
		avg_color[1]= GETBITSHIGH(block_part1, 4, 55);
		avg_color[2]= GETBITSHIGH(block_part1, 4, 47);

		// Here, we should really multiply by 17 instead of 16. This can
		// be done by just copying the four lower bits to the upper ones
		// while keeping the lower bits.
		avg_color[0] |= (avg_color[0] <<4);
		avg_color[1] |= (avg_color[1] <<4);
		avg_color[2] |= (avg_color[2] <<4);

		table = GETBITSHIGH(block_part1, 3, 39) << 1;



		pixel_indices_MSB = GETBITS(block_part2, 16, 31);
		pixel_indices_LSB = GETBITS(block_part2, 16, 15);

		if( (flipbit) == 0 )
		{
			// We should not flip
			shift = 0;
			for(x=startx; x<startx+2; x++)
			{
				for(y=starty; y<starty+4; y++)
				{
					index  = ((pixel_indices_MSB >> shift) & 1) << 1;
					index |= ((pixel_indices_LSB >> shift) & 1);
					shift++;
					index=unscramble[index];

					r=RED(img,width,x,y)  =CLAMP(0,avg_color[0]+compressParams[table][index],255);
					g=GREEN(img,width,x,y)=CLAMP(0,avg_color[1]+compressParams[table][index],255);
					b=BLUE(img,width,x,y) =CLAMP(0,avg_color[2]+compressParams[table][index],255);


				}
			}
		}
		else
		{
			// We should flip
			shift = 0;
			for(x=startx; x<startx+4; x++)
			{
				for(y=starty; y<starty+2; y++)
				{
					index  = ((pixel_indices_MSB >> shift) & 1) << 1;
					index |= ((pixel_indices_LSB >> shift) & 1);
					shift++;
					index=unscramble[index];

					r=RED(img,width,x,y)  =CLAMP(0,avg_color[0]+compressParams[table][index],255);
					g=GREEN(img,width,x,y)=CLAMP(0,avg_color[1]+compressParams[table][index],255);
					b=BLUE(img,width,x,y) =CLAMP(0,avg_color[2]+compressParams[table][index],255);
				}
				shift+=2;
			}
		}

		// Now decode other part of block. 
		avg_color[0]= GETBITSHIGH(block_part1, 4, 59);
		avg_color[1]= GETBITSHIGH(block_part1, 4, 51);
		avg_color[2]= GETBITSHIGH(block_part1, 4, 43);

		// Here, we should really multiply by 17 instead of 16. This can
		// be done by just copying the four lower bits to the upper ones
		// while keeping the lower bits.
		avg_color[0] |= (avg_color[0] <<4);
		avg_color[1] |= (avg_color[1] <<4);
		avg_color[2] |= (avg_color[2] <<4);

		table = GETBITSHIGH(block_part1, 3, 36) << 1;
		pixel_indices_MSB = GETBITS(block_part2, 16, 31);
		pixel_indices_LSB = GETBITS(block_part2, 16, 15);

		if( (flipbit) == 0 )
		{
			// We should not flip
			shift=8;
			for(x=startx+2; x<startx+4; x++)
			{
				for(y=starty; y<starty+4; y++)
				{
					index  = ((pixel_indices_MSB >> shift) & 1) << 1;
					index |= ((pixel_indices_LSB >> shift) & 1);
					shift++;
					index=unscramble[index];

					r=RED(img,width,x,y)  =CLAMP(0,avg_color[0]+compressParams[table][index],255);
					g=GREEN(img,width,x,y)=CLAMP(0,avg_color[1]+compressParams[table][index],255);
					b=BLUE(img,width,x,y) =CLAMP(0,avg_color[2]+compressParams[table][index],255);

				}
			}
		}
		else
		{
			// We should flip
			shift=2;
			for(x=startx; x<startx+4; x++)
			{
				for(y=starty+2; y<starty+4; y++)
				{
					index  = ((pixel_indices_MSB >> shift) & 1) << 1;
					index |= ((pixel_indices_LSB >> shift) & 1);
					shift++;
					index=unscramble[index];

					r=RED(img,width,x,y)  =CLAMP(0,avg_color[0]+compressParams[table][index],255);
					g=GREEN(img,width,x,y)=CLAMP(0,avg_color[1]+compressParams[table][index],255);
					b=BLUE(img,width,x,y) =CLAMP(0,avg_color[2]+compressParams[table][index],255);

				}
				shift += 2;
			}
		}

	}
	else
	{
		// We have diffbit = 1. 

		//      63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34  33  32 
		//      ---------------------------------------------------------------------------------------------------
		//     | base col1    | dcol 2 | base col1    | dcol 2 | base col 1   | dcol 2 | table  | table  |diff|flip|
		//     | R1' (5 bits) | dR2    | G1' (5 bits) | dG2    | B1' (5 bits) | dB2    | cw 1   | cw 2   |bit |bit |
		//      ---------------------------------------------------------------------------------------------------
		// 
		// 
		//     c) bit layout in bits 31 through 0 (in both cases)
		// 
		//      31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3   2   1  0
		//      --------------------------------------------------------------------------------------------------
		//     |       most significant pixel index bits       |         least significant pixel index bits       |  
		//     | p| o| n| m| l| k| j| i| h| g| f| e| d| c| b| a| p| o| n| m| l| k| j| i| h| g| f| e| d| c | b | a |
		//      --------------------------------------------------------------------------------------------------      


		// First decode left part of block.
		enc_color1[0]= GETBITSHIGH(block_part1, 5, 63);
		enc_color1[1]= GETBITSHIGH(block_part1, 5, 55);
		enc_color1[2]= GETBITSHIGH(block_part1, 5, 47);


		// Expand from 5 to 8 bits
		avg_color[0] = (enc_color1[0] <<3) | (enc_color1[0] >> 2);
		avg_color[1] = (enc_color1[1] <<3) | (enc_color1[1] >> 2);
		avg_color[2] = (enc_color1[2] <<3) | (enc_color1[2] >> 2);


		table = GETBITSHIGH(block_part1, 3, 39) << 1;

		pixel_indices_MSB = GETBITS(block_part2, 16, 31);
		pixel_indices_LSB = GETBITS(block_part2, 16, 15);

		if( (flipbit) == 0 )
		{
			// We should not flip
			shift = 0;
			for(x=startx; x<startx+2; x++)
			{
				for(y=starty; y<starty+4; y++)
				{
					index  = ((pixel_indices_MSB >> shift) & 1) << 1;
					index |= ((pixel_indices_LSB >> shift) & 1);
					shift++;
					index=unscramble[index];

					r=RED(img,width,x,y)  =CLAMP(0,avg_color[0]+compressParams[table][index],255);
					g=GREEN(img,width,x,y)=CLAMP(0,avg_color[1]+compressParams[table][index],255);
					b=BLUE(img,width,x,y) =CLAMP(0,avg_color[2]+compressParams[table][index],255);


				}
			}
		}
		else
		{
			// We should flip
			shift = 0;
			for(x=startx; x<startx+4; x++)
			{
				for(y=starty; y<starty+2; y++)
				{
					index  = ((pixel_indices_MSB >> shift) & 1) << 1;
					index |= ((pixel_indices_LSB >> shift) & 1);
					shift++;
					index=unscramble[index];

					r=RED(img,width,x,y)  =CLAMP(0,avg_color[0]+compressParams[table][index],255);
					g=GREEN(img,width,x,y)=CLAMP(0,avg_color[1]+compressParams[table][index],255);
					b=BLUE(img,width,x,y) =CLAMP(0,avg_color[2]+compressParams[table][index],255);
				}
				shift+=2;
			}
		}


		// Now decode right part of block. 


		diff[0]= GETBITSHIGH(block_part1, 3, 58);
		diff[1]= GETBITSHIGH(block_part1, 3, 50);
		diff[2]= GETBITSHIGH(block_part1, 3, 42);

		enc_color2[0]= enc_color1[0] + diff[0];
		enc_color2[1]= enc_color1[1] + diff[1];
		enc_color2[2]= enc_color1[2] + diff[2];

		// Extend sign bit to entire byte. 
		diff[0] = (diff[0] << 5);
		diff[1] = (diff[1] << 5);
		diff[2] = (diff[2] << 5);
		diff[0] = diff[0] >> 5;
		diff[1] = diff[1] >> 5;
		diff[2] = diff[2] >> 5;

		//  Calculale second color
		enc_color2[0]= enc_color1[0] + diff[0];
		enc_color2[1]= enc_color1[1] + diff[1];
		enc_color2[2]= enc_color1[2] + diff[2];

		// Expand from 5 to 8 bits
		avg_color[0] = (enc_color2[0] <<3) | (enc_color2[0] >> 2);
		avg_color[1] = (enc_color2[1] <<3) | (enc_color2[1] >> 2);
		avg_color[2] = (enc_color2[2] <<3) | (enc_color2[2] >> 2);


		table = GETBITSHIGH(block_part1, 3, 36) << 1;
		pixel_indices_MSB = GETBITS(block_part2, 16, 31);
		pixel_indices_LSB = GETBITS(block_part2, 16, 15);

		if( (flipbit) == 0 )
		{
			// We should not flip
			shift=8;
			for(x=startx+2; x<startx+4; x++)
			{
				for(y=starty; y<starty+4; y++)
				{
					index  = ((pixel_indices_MSB >> shift) & 1) << 1;
					index |= ((pixel_indices_LSB >> shift) & 1);
					shift++;
					index=unscramble[index];

					r=RED(img,width,x,y)  =CLAMP(0,avg_color[0]+compressParams[table][index],255);
					g=GREEN(img,width,x,y)=CLAMP(0,avg_color[1]+compressParams[table][index],255);
					b=BLUE(img,width,x,y) =CLAMP(0,avg_color[2]+compressParams[table][index],255);

				}
			}
		}
		else
		{
			// We should flip
			shift=2;
			for(x=startx; x<startx+4; x++)
			{
				for(y=starty+2; y<starty+4; y++)
				{
					index  = ((pixel_indices_MSB >> shift) & 1) << 1;
					index |= ((pixel_indices_LSB >> shift) & 1);
					shift++;
					index=unscramble[index];

					r=RED(img,width,x,y)  =CLAMP(0,avg_color[0]+compressParams[table][index],255);
					g=GREEN(img,width,x,y)=CLAMP(0,avg_color[1]+compressParams[table][index],255);
					b=BLUE(img,width,x,y) =CLAMP(0,avg_color[2]+compressParams[table][index],255);
				}
				shift += 2;
			}
		}
	}
}
/*
ILuint low_endian_to_big_endian_4byte_word(ILuint* inValue)
{
	ILuint block = 0;
	ILuint a=*inValue;
	ILuint b1,b2,b3,b4;

	b1=a<<24;
	b2=a<<8&0x00ff0000;
	b3=a>>24;
	b4=a>>8&0x0000ff00;
	block=b1|b2|b3|b4;

	return block;
}
*/
ILvoid read_big_endian_4byte_word(ILuint *blockadr)
{
	ILubyte bytes[4];
	ILuint block;

	iread(&bytes[0], 1, 1);
	iread(&bytes[1], 1, 1);
	iread(&bytes[2], 1, 1);
	iread(&bytes[3], 1, 1);


	block = 0;

	block |= bytes[0];
	block = block << 8;
	block |= bytes[1];
	block = block << 8;
	block |= bytes[2];
	block = block << 8;
	block |= bytes[3];

	blockadr[0] = block;
}

ILAPI ILvoid ILAPIENTRY ilSetEtcDecompress(ILboolean bDecompress)
{
	 bDecompressEtc = bDecompress;
}

#endif//IL_NO_KTX