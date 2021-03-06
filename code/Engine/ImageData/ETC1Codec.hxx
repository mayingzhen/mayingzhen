#include "ETC1Codec.h"
namespace ma
{

#define FOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))

const uint32_t ETC1_MAGIC = FOURCC('P', 'K', 'M', ' ');

typedef struct {
	int8_t   aName[6];
	uint16_t iBlank;
	uint8_t  iPaddedWidthMSB;
	uint8_t  iPaddedWidthLSB;
	uint8_t  iPaddedHeightMSB;
	uint8_t  iPaddedHeightLSB;
	uint8_t  iWidthMSB;
	uint8_t  iWidthLSB;
	uint8_t  iHeightMSB;
	uint8_t  iHeightLSB;
} ETCHeader;

//---------------------------------------------------------------------
CETC1Codec::CETC1Codec():
mType("pkm")
{ 
}

CETC1Codec::~CETC1Codec()
{

}
//---------------------------------------------------------------------
/*DataStreamPtr CETC1Codec::code(MemoryDataStreamPtr& input, Codec::CodecDataPtr& pData) const
{        
	OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
		"ETC1 encoding not supported",
		"CETC1Codec::code" ) ;
}
//---------------------------------------------------------------------
void CETC1Codec::codeToFile(MemoryDataStreamPtr& input, 
	const string& outFileName, Codec::CodecDataPtr& pData) const
{
	OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
		"ETC1 encoding not supported",
		"CETC1Codec::codeToFile" ) ;
}
//---------------------------------------------------------------------
Codec::DecodeResult CETC1Codec::decode(DataStreamPtr& stream) const*/
bool CETC1Codec::decode(const char* pszName, void* pMemory, uint32_t nNumBytes, IN OUT ImageData& imgData) const
{
	ETCHeader header;

	MemoryStream stream(pszName, pMemory, nNumBytes, true);

	// Read the ETC1 header
	stream.Read(&header, sizeof(ETCHeader));

	if (ETC1_MAGIC != FOURCC(header.aName[0], header.aName[1], header.aName[2], header.aName[3]) ) // "PKM 10"
	{
		LogError("CETC1Codec::decode, This is not a ETC1 file!, %s", pszName);
		return false;
	}

	// TODO add endian awareness
	uint16_t width = (header.iWidthMSB << 8) | header.iWidthLSB;
	uint16_t height = (header.iHeightMSB << 8) | header.iHeightLSB;
	uint16_t paddedWidth = (header.iPaddedWidthMSB << 8) | header.iPaddedWidthLSB;
	uint16_t paddedHeight = (header.iPaddedHeightMSB << 8) | header.iPaddedHeightLSB;

	imgData.m_nDepth = 1;
	imgData.m_nWidth = width;
	imgData.m_nHeight = height;
	imgData.m_eFormat = PF_ETC1_RGB8;

	// ETC1 has no support for mipmaps - malideveloper.com has a example 
	// where the load mipmap levels from different external files
	imgData.m_nNumMipmaps = 0; 

	// ETC1 is a compressed format
	imgData.m_nFlags |= IF_COMPRESSED;


	// Calculate total size from number of mipmaps, faces and size
	imgData.m_nSize = (paddedWidth * paddedHeight) >> 1;

	RefPtr<MemoryStream> output;
	// Bind output buffer
	output = CreateMemoryStream(imgData.m_nSize, false);

	// Now deal with the data
	void *destPtr = output->GetPtr();
	stream.Read(destPtr, imgData.m_nSize);
	destPtr = static_cast<void*>(static_cast<uint8_t*>(destPtr));

	imgData.m_pMemory = output;
	return true;
}
//---------------------------------------------------------------------    
string CETC1Codec::getType() const 
{
	return mType;
}
//---------------------------------------------------------------------    
void CETC1Codec::flipEndian(void * pData, size_t size, size_t count) const
{
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
	for(unsigned int index = 0; index < count; index++)
	{
		flipEndian((void *)((long)pData + (index * size)), size);
	}
#endif
}
//---------------------------------------------------------------------    
void CETC1Codec::flipEndian(void * pData, size_t size) const
{
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
	char swapByte;
	for(unsigned int byteIndex = 0; byteIndex < size/2; byteIndex++)
	{
		swapByte = *(char *)((long)pData + byteIndex);
		*(char *)((long)pData + byteIndex) = *(char *)((long)pData + size - byteIndex - 1);
		*(char *)((long)pData + size - byteIndex - 1) = swapByte;
	}
#endif
}
//---------------------------------------------------------------------    
string CETC1Codec::magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const
{
	if (maxbytes >= sizeof(uint32_t))
	{
		uint32_t fileType;
		memcpy(&fileType, magicNumberPtr, sizeof(uint32_t));
		flipEndian(&fileType, sizeof(uint32_t), 1);

		if (ETC1_MAGIC == fileType)
		{
			return "pkm";
		}
	}

	return StringUtil::BLANK;
}

}