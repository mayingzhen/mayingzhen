#include "il_mjp.h"
#include "zlib.h"

typedef struct _MJPG_HEADER
{	
	char cFlags[4];//�ļ���־
	int nJpgDataPos;//jpg����λ��
	int nJpgDataLen;//jpg���ݳ���
	int nJpgZipDataLen;//jpgѹ�����ֳ���(jpg���ݲ��ֿ�ͷ1K����ѹ������)
	int nJpgUnZipDataLen;//jpgѹ�����ֽ�ѹ��ĳ���
	int nAlphaDataPos;//ͨ������λ��
	int nAlphaDataLen;//ͨ�����ݳ���
	int nAlphaUnzipDataLen;//δѹ��ǰͨ�����ݳ���
	int nAlphaBitCount;//ͨ��λ��
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
			//����δѹ��
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
	
	// ��ѹjpgͷ����
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
	
	// �õ�����jpg����
	if(header.nJpgDataLen > header.nJpgZipDataLen)
	{
		iread(pRawJpgData + header.nJpgUnZipDataLen, 1, header.nJpgDataLen - header.nJpgZipDataLen);
	}
	
	// ����Alpha����
	if(header.nAlphaDataLen > 0)
	{
		pZipedAlphaData = ialloc(header.nAlphaDataLen);
		iseek(header.nAlphaDataPos, IL_SEEK_SET);
		iread(pZipedAlphaData, 1, header.nAlphaDataLen);
	}
	 
	// ����jpgͼ�����ݵ�iCurImage
	ilLoadJpegL(pRawJpgData, nRawJpgLen);

	// ���Alphaͨ��
	ilAddAlpha();
	
	// �ͷ��м��ڴ�
	ifree(pRawJpgData);
	ifree(pZipedJpgData);
	pRawJpgData = NULL;
	pZipedJpgData = NULL;
	
	if(header.nAlphaDataLen <= 0)
	{
		// û��Alpha���ݽ������.
		return IL_TRUE;
	}
	
	// ��ѹAlpha����
	pRawAlphaData = ialloc(header.nAlphaUnzipDataLen);
	iUnZipJPGData(pZipedAlphaData, header.nAlphaDataLen, pRawAlphaData, header.nAlphaUnzipDataLen);
	ifree(pZipedAlphaData);
	pZipedAlphaData = NULL;
	
	nPixelCount = iCurImage->Width * iCurImage->Height;
	
	// ��ͨ������д��iCurImage
	switch(header.nAlphaBitCount)
	{
	case 1:
		{	
			n = nPixelCount / 8;// ��Ҫʹ�ö��ٸ������ֽ�
			for(i = 0; i < n; i++)
			{	
				for(j = 0; j < 8; j++)
				{	
					pPixelData = iCurImage->Data + (i * 8 + j) * 4;
					pPixelData[3] = (pRawAlphaData[i] & (1 << j))? 0xFF : 0;
				}
			}
			
			// ʣ�µ��ֽ�λ
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
			n = nPixelCount / 2;// ��Ҫʹ�ö��ٸ������ֽ�
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
			 
			n = nPixelCount % 2;// ʣ�µİ���ֽ�
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