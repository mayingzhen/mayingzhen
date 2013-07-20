#include "il_mjp.h"
#include "zlib.h"

typedef struct _MJPG_HEADER
{	
	char cFlags[4];//文件标志
	int nJpgDataPos;//jpg数据位置
	int nJpgDataLen;//jpg数据长度
	int nJpgZipDataLen;//jpg压缩部分长度(jpg数据部分开头1K数据压缩加密)
	int nJpgUnZipDataLen;//jpg压缩部分解压后的长度
	int nAlphaDataPos;//通道数据位置
	int nAlphaDataLen;//通道数据长度
	int nAlphaUnzipDataLen;//未压缩前通道数据长度
	int nAlphaBitCount;//通道位数
}	MJPG_HEADER;

ILboolean ilLoadMjpL(const ILvoid* Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return ilLoadMpjInternal();
}

ILboolean ilLoadMjpF(ILHANDLE File)
{
	ILuint FirstPos;
	ILboolean bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	
	bRet = ilLoadMpjInternal();
	iseek(FirstPos, IL_SEEK_SET);
	return bRet;
}

ILboolean ilLoadMjp(const ILstring FileName)
{
	ILHANDLE hMjpFile;
	ILboolean bMjp = IL_FALSE;
	
	hMjpFile = iopenr(FileName);
	if(hMjpFile == NULL)
	{
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bMjp;
	}
	
	bMjp = ilLoadMjpF(hMjpFile);
	icloser(hMjpFile);
	
	return bMjp;
}

ILboolean iGetMpjHeader(MJPG_HEADER* header)
{
	iread(header->cFlags, 1, 4);
	header->nJpgDataPos = GetLittleUInt();
	header->nJpgDataLen = GetLittleUInt();
	header->nJpgZipDataLen = GetLittleUInt();
	header->nJpgUnZipDataLen= GetLittleUInt();
	header->nAlphaDataPos = GetLittleUInt();
	header->nAlphaDataLen = GetLittleUInt();
	header->nAlphaUnzipDataLen = GetLittleUInt();
	header->nAlphaBitCount = GetLittleUInt();
	return IL_TRUE;
}

ILboolean iUnZipJPGData(void* pSrcData, int nSrcLen, void* pDestData, int nDestLen)
{
	int nRet;
	int nUncompLen;
	
	nUncompLen = nDestLen;
	nRet = uncompress(pDestData, &nUncompLen, pSrcData, nSrcLen);
	if(Z_OK != nRet)
	{
		if(nSrcLen == nDestLen)
		{	
			//数据未压缩
			memcpy(pDestData, pSrcData, nSrcLen);
		}
		else return IL_FALSE;
	}
	return IL_TRUE;
}

ILboolean ilLoadMpjInternal()
{
	int i, j, n;
	int nPixelCount;
	int nRawJpgLen;
	MJPG_HEADER header;
	void* pZipedJpgData;
	ILubyte* pRawJpgData;
	ILubyte* pZipedAlphaData;
	ILubyte* pRawAlphaData;
	ILubyte* pPixelData;

	if(iCurImage == NULL)
	{
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	
	if(!iGetMpjHeader(&header))
	{
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}
	
	// 解压jpg头数据
	nRawJpgLen = header.nJpgUnZipDataLen;
	if(header.nJpgDataLen > header.nJpgZipDataLen)
	{
		nRawJpgLen += (header.nJpgDataLen - header.nJpgZipDataLen);
	}

	pZipedJpgData = ialloc(header.nJpgZipDataLen);
	iseek(header.nJpgDataPos, IL_SEEK_SET);
	iread(pZipedJpgData, 1, header.nJpgZipDataLen);
		
	pRawJpgData = (ILubyte*)ialloc(nRawJpgLen);
	iUnZipJPGData(pZipedJpgData, header.nJpgZipDataLen, pRawJpgData, header.nJpgUnZipDataLen);
	
	// 得到所有jpg数据
	if(header.nJpgDataLen > header.nJpgZipDataLen)
	{
		iread(pRawJpgData + header.nJpgUnZipDataLen, 1, header.nJpgDataLen - header.nJpgZipDataLen);
	}
	
	// 读出Alpha数据
	if(header.nAlphaDataLen > 0)
	{
		pZipedAlphaData = ialloc(header.nAlphaDataLen);
		iseek(header.nAlphaDataPos, IL_SEEK_SET);
		iread(pZipedAlphaData, 1, header.nAlphaDataLen);
	}
	 
	// 解析jpg图像数据到iCurImage
	ilLoadJpegL(pRawJpgData, nRawJpgLen);

	// 添加Alpha通道
	ilAddAlpha();
	
	// 释放中间内存
	ifree(pRawJpgData);
	ifree(pZipedJpgData);
	pRawJpgData = NULL;
	pZipedJpgData = NULL;
	
	if(header.nAlphaDataLen <= 0)
	{
		// 没有Alpha数据解析完毕.
		return IL_TRUE;
	}
	
	// 解压Alpha数据
	pRawAlphaData = ialloc(header.nAlphaUnzipDataLen);
	iUnZipJPGData(pZipedAlphaData, header.nAlphaDataLen, pRawAlphaData, header.nAlphaUnzipDataLen);
	ifree(pZipedAlphaData);
	pZipedAlphaData = NULL;
	
	nPixelCount = iCurImage->Width * iCurImage->Height;
	
	// 将通道数据写入iCurImage
	switch(header.nAlphaBitCount)
	{
	case 1:
		{	
			n = nPixelCount / 8;// 需要使用多少个完整字节
			for(i = 0; i < n; i++)
			{	
				for(j = 0; j < 8; j++)
				{	
					pPixelData = iCurImage->Data + (i * 8 + j) * 4;
					pPixelData[3] = (pRawAlphaData[i] & (1 << j))? 0xFF : 0;
				}
			}
			
			// 剩下的字节位
			n = nPixelCount % 8;
			for(j = 0; j < n; j++)
			{	
				pPixelData = iCurImage->Data + (i * 8 + j) * 4;
				pPixelData[3] = (pRawAlphaData[i] & (1 << j))? 0xFF : 0;
			}
		}
		break;
		
	case 2:
		{	
			n = nPixelCount / 2;// 需要使用多少个完整字节
			for(i = 0; i < n; i++)
			{
				pPixelData = iCurImage->Data + i * 2 * 4;
				pPixelData[3] = pRawAlphaData[i] << 4;
				if(pPixelData[3] == 0xF0)
				{
					pPixelData[3] = 0xFF;
				}
				pPixelData += 4;
				pPixelData[3] = (pRawAlphaData[i] >> 4 << 4);
				if(pPixelData[3] == 0xF0)
				{
					pPixelData[3] = 0xFF;
				}
			}
			 
			n = nPixelCount % 2;// 剩下的半个字节
			if(n != 0)
			{	 
				pPixelData = iCurImage->Data + i * 2 * 4;
				pPixelData[3] = pRawAlphaData[i] << 4;
				if(pPixelData[3] == 0xF0)
				{
					pPixelData[3] = 0xFF;
				}
			}
		} 
		break;
	
	case 3:
		{
			for(i = 0; i < header.nAlphaUnzipDataLen; i++)
			{
				pPixelData = iCurImage->Data + i * 4;
				pPixelData[3] = pRawAlphaData[i];
			}
		}
		break;

	default:
		{
			ifree(pRawAlphaData);
			pRawAlphaData = NULL;
			return IL_FALSE;
		}
		break;
	} 
	
	ifree(pRawAlphaData);
	pRawAlphaData = NULL;
	return IL_TRUE;
}