#include "PVRTCCodec.h"
#include "PixelFormat.h"

#define FOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))
#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff

namespace ma {
	
#ifdef WIN32
#pragma pack (push, 1)
#else
#pragma pack (1)
#endif

	const uint32_t PVR2_MAGIC = FOURCC('P', 'V', 'R', '!'); 
    const uint32_t PVR3_MAGIC = FOURCC('P', 'V', 'R', 3); 

	enum
    {
		kPVRTextureFlagTypeRGBA_4444= 0x10,
		kPVRTextureFlagTypeRGBA_5551,
		kPVRTextureFlagTypeRGBA_8888,
		kPVRTextureFlagTypeRGB_565,
		kPVRTextureFlagTypeRGB_555,				// unsupported
		kPVRTextureFlagTypeRGB_888,
		kPVRTextureFlagTypeI_8,
		kPVRTextureFlagTypeAI_88,
		kPVRTextureFlagTypePVRTC_2,
		kPVRTextureFlagTypePVRTC_4,
		kPVRTextureFlagTypeBGRA_8888,
		kPVRTextureFlagTypeA_8,
		kPVRTextureFlagTypeETC = 54,
    };

    enum
    {
        kPVRTC1_PF_2BPP_RGB,
        kPVRTC1_PF_2BPP_RGBA,
        kPVRTC1_PF_4BPP_RGB,
        kPVRTC1_PF_4BPP_RGBA,
        kPVRTC2_PF_2BPP,
        kPVRTC2_PF_4BPP
    };

	typedef struct _PVRTCTexHeaderV2
    {
        uint32_t headerLength;
        uint32_t height;
        uint32_t width;
        uint32_t numMipmaps;
        uint32_t flags;
        uint32_t dataLength;
        uint32_t bpp;
        uint32_t bitmaskRed;
        uint32_t bitmaskGreen;
        uint32_t bitmaskBlue;
        uint32_t bitmaskAlpha;
        uint32_t pvrTag;
        uint32_t numSurfs;
    } PVRTCTexHeaderV2;

    typedef struct _PVRTCTexHeaderV3
    {
        uint32_t  version;         //Version of the file header, used to identify it.
        uint32_t  flags;           //Various format flags.
        uint64_t  pixelFormat;     //The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
        uint32_t  colourSpace;     //The Colour Space of the texture, currently either linear RGB or sRGB.
        uint32_t  channelType;     //Variable type that the channel is stored in. Supports signed/unsigned int/short/byte or float for now.
        uint32_t  height;          //Height of the texture.
        uint32_t  width;           //Width of the texture.
        uint32_t  depth;           //Depth of the texture. (Z-slices)
        uint32_t  numSurfaces;     //Number of members in a Texture Array.
        uint32_t  numFaces;        //Number of faces in a Cube Map. Maybe be a value other than 6.
        uint32_t  mipMapCount;     //Number of MIP Maps in the texture - NB: Includes top level.
        uint32_t  metaDataSize;    //Size of the accompanying meta data.
    } PVRTCTexHeaderV3;

    typedef struct _PVRTCMetaData
    {
        uint32_t DevFOURCC;
        uint32_t u32Key;
        uint32_t u32DataSize;
        uint8_t* Data;
    } PVRTCMetadata;
	
#ifdef WIN32
#pragma pack (pop)
#else
#pragma pack ()
#endif

	//---------------------------------------------------------------------
    CPVRTCCodec::CPVRTCCodec():
        mType("pvr")
    { 
    }
    //---------------------------------------------------------------------
    /*DataStreamPtr PVRTCCodec::code(MemoryDataStreamPtr& input, Codec::CodecDataPtr& pData) const
    {        
		OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
                    "PVRTC encoding not supported",
                    "PVRTCCodec::code" ) ;
    }
    //---------------------------------------------------------------------
    void PVRTCCodec::codeToFile(MemoryDataStreamPtr& input, 
        const string& outFileName, Codec::CodecDataPtr& pData) const
    {
		OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
                    "PVRTC encoding not supported",
                    "PVRTCCodec::codeToFile" ) ;
	}
    //---------------------------------------------------------------------
    Codec::DecodeResult PVRTCCodec::decode(DataStreamPtr& stream) const*/
	bool CPVRTCCodec::decode(const char* pszName, void* pMemory, uint32_t nNumBytes, IN OUT ImageData& imgData) const
	{
		MemoryStream stream(pszName, pMemory, nNumBytes, true);
		// Assume its a pvr 2 header
		PVRTCTexHeaderV2 headerV2;
		stream.Read(&headerV2, sizeof(PVRTCTexHeaderV2));
		stream.Seek(0);

		if (PVR2_MAGIC == headerV2.pvrTag)
		{			
			return decodeV2(&stream, imgData);
		}

		// Try it as pvr 3 header
		PVRTCTexHeaderV3 headerV3;
		stream.Read(&headerV3, sizeof(PVRTCTexHeaderV3));
		stream.Seek(0);

		if (PVR3_MAGIC == headerV3.version)
		{
			return decodeV3(&stream, imgData);
		}

		LogError("This is not a PVR2 / PVR3 file!, PVRTCCodec::decode");
		return false;
    }
    //---------------------------------------------------------------------    
	bool CPVRTCCodec::decodeV2(MemoryStream* stream, IN OUT ImageData& imgData) const
	{
		PVRTCTexHeaderV2 header;
        uint32_t flags = 0, formatFlags = 0;
        size_t numFaces = 1; // Assume one face until we know otherwise

        // Read the PVRTC header
        stream->Read(&header, sizeof(PVRTCTexHeaderV2));

        // Get format flags
        flags = header.flags;
        flipEndian((void *)flags, sizeof(uint32_t));
        formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;

        uint32_t bitmaskAlpha = header.bitmaskAlpha;
        flipEndian((void *)bitmaskAlpha, sizeof(uint32_t));

        imgData.m_nDepth = 1;
        imgData.m_nWidth = header.width;
        imgData.m_nHeight = header.height;
        imgData.m_nNumMipmaps = static_cast<uint16_t>(header.numMipmaps);

		switch(formatFlags)
		{
		case kPVRTextureFlagTypeRGBA_4444:
			imgData.m_eFormat = PF_R4G4B4A4;
			break;
		case kPVRTextureFlagTypeRGBA_5551:
			imgData.m_eFormat = PF_A1R5G5B5;
			break;
		case kPVRTextureFlagTypeRGBA_8888:
			imgData.m_eFormat = PF_A8B8G8R8;
			break;
		case kPVRTextureFlagTypeRGB_565:
			imgData.m_eFormat = PF_R5G6B5;
			break;
		case kPVRTextureFlagTypeRGB_888:
			imgData.m_eFormat = PF_B8G8R8;
			break;
		case kPVRTextureFlagTypeI_8:
			imgData.m_eFormat = PF_L8;
			break;
		case kPVRTextureFlagTypeAI_88:
			imgData.m_eFormat = PF_BYTE_LA;
			break;
		case kPVRTextureFlagTypePVRTC_2:
			imgData.m_eFormat = bitmaskAlpha ? PF_PVRTC_RGBA2 : PF_PVRTC_RGB2;
			imgData.m_nFlags |= IF_COMPRESSED;
			break;
		case kPVRTextureFlagTypePVRTC_4:
			imgData.m_eFormat = bitmaskAlpha ? PF_PVRTC_RGBA4 : PF_PVRTC_RGB4;
			imgData.m_nFlags |= IF_COMPRESSED;
			break;
		case kPVRTextureFlagTypeBGRA_8888:
			imgData.m_eFormat = PF_A8R8G8B8;
			break;
		case kPVRTextureFlagTypeA_8:
			imgData.m_eFormat = PF_A8;
			break;
		case kPVRTextureFlagTypeETC:
			ASSERT(bitmaskAlpha == false);
			imgData.m_eFormat = PF_ETC1_RGB8;
			imgData.m_nFlags |= IF_COMPRESSED;
			break;
		case kPVRTextureFlagTypeRGB_555://Unsupport
		default:
			LogError("UnSupport file: %s, formatFlags:%d", stream->GetName(), formatFlags);
			return false;
			break;
		}

        // Calculate total size from number of mipmaps, faces and size
		imgData.m_nSize = CImageCodec::calculateSize(imgData.m_nNumMipmaps, numFaces, 
                                             imgData.m_nWidth, imgData.m_nHeight, imgData.m_nDepth, imgData.m_eFormat);

		RefPtr<MemoryStream> output;
		// Bind output buffer
		output = CreateMemoryStream(imgData.m_nSize, false);

		// Now deal with the data
		void *destPtr = output->GetPtr();
        stream->Read(destPtr, imgData.m_nSize);
        destPtr = static_cast<void*>(static_cast<uint8_t*>(destPtr));

		imgData.m_pMemory = output;
		return true;
    }

    //---------------------------------------------------------------------  
	bool CPVRTCCodec::decodeV3(MemoryStream* stream, IN OUT ImageData& imgData) const
	{
		PVRTCTexHeaderV3 header;
		PVRTCMetadata metadata;
		uint32_t flags = 0;
		size_t numFaces = 1; // Assume one face until we know otherwise

		// Read the PVRTC header
		stream->Read(&header, sizeof(PVRTCTexHeaderV3));

		// Read the PVRTC metadata
		if(header.metaDataSize)
		{
			stream->Read(&metadata, sizeof(PVRTCMetadata));
		}

		// Identify the pixel format
		switch (header.pixelFormat)
		{
		case kPVRTC1_PF_2BPP_RGB:
			imgData.m_eFormat = PF_PVRTC_RGB2;
			break;
		case kPVRTC1_PF_2BPP_RGBA:
			imgData.m_eFormat = PF_PVRTC_RGBA2;
			break;
		case kPVRTC1_PF_4BPP_RGB:
			imgData.m_eFormat = PF_PVRTC_RGB4;
			break;
		case kPVRTC1_PF_4BPP_RGBA:
			imgData.m_eFormat = PF_PVRTC_RGBA4;
			break;
		case kPVRTC2_PF_2BPP:
			imgData.m_eFormat = PF_PVRTC2_2BPP;
			break;
		case kPVRTC2_PF_4BPP:
			imgData.m_eFormat = PF_PVRTC2_4BPP;
			break;
		}

		// Get format flags
		flags = header.flags;
		flipEndian((void *)flags, sizeof(uint32_t));

		imgData.m_nDepth = header.depth;
		imgData.m_nWidth = header.width;
		imgData.m_nHeight = header.height;
		ASSERT(header.mipMapCount >= 1);
		imgData.m_nNumMipmaps = static_cast<uint16_t>(header.mipMapCount-1);//v3格式要减去1，v2不需要

		// PVRTC is a compressed format
		imgData.m_nFlags |= IF_COMPRESSED;

		if(header.numFaces == 6)
			imgData.m_nFlags |= IF_CUBEMAP;

		if(header.depth > 1)
			imgData.m_nFlags |= IF_3D_TEXTURE;

		// Calculate total size from number of mipmaps, faces and size
		imgData.m_nSize = CImageCodec::calculateSize(imgData.m_nNumMipmaps, numFaces, 
			imgData.m_nWidth, imgData.m_nHeight, imgData.m_nDepth, imgData.m_eFormat);

		// Bind output buffer
		RefPtr<MemoryStream> output = CreateMemoryStream(imgData.m_nSize, false);

		// Now deal with the data
		void *destPtr = output->GetPtr();

		size_t width = imgData.m_nWidth;
		size_t height = imgData.m_nHeight;
		size_t depth = imgData.m_nDepth;

		// All mips for a surface, then each face
		for(size_t mip = 0; mip <= imgData.m_nNumMipmaps; ++mip)
		{
			for(size_t surface = 0; surface < header.numSurfaces; ++surface)
			{
				for(size_t i = 0; i < numFaces; ++i)
				{
					// Load directly
					size_t pvrSize = PixelUtil::getMemorySize(width, height, depth, imgData.m_eFormat);
					stream->Read(destPtr, pvrSize);
					destPtr = static_cast<void*>(static_cast<uint8_t*>(destPtr) + pvrSize);
				}
			}

			// Next mip
			if(width!=1) width /= 2;
			if(height!=1) height /= 2;
			if(depth!=1) depth /= 2;
		}

		imgData.m_pMemory = output;
		return true;
	}
	//---------------------------------------------------------------------   
    string CPVRTCCodec::getType() const 
    {
        return mType;
    }
    //---------------------------------------------------------------------    
    void CPVRTCCodec::flipEndian(void * pData, size_t size, size_t count) const
    {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
		for(unsigned int index = 0; index < count; index++)
        {
            flipEndian((void *)((long)pData + (index * size)), size);
        }
#endif
    }
    //---------------------------------------------------------------------    
    void CPVRTCCodec::flipEndian(void * pData, size_t size) const
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
	string CPVRTCCodec::magicNumberToFileExt(const char *magicNumberPtr, size_t maxbytes) const
	{
		if (maxbytes >= sizeof(uint32_t))
		{
			uint32_t fileType;
			memcpy(&fileType, magicNumberPtr, sizeof(uint32_t));
			flipEndian(&fileType, sizeof(uint32_t), 1);

			if (PVR3_MAGIC == fileType || PVR2_MAGIC == fileType)
			{
				return string("pvr");
			}
		}

		return StringUtil::BLANK;
	}
}
