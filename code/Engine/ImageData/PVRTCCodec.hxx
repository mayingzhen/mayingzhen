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

	const uint32 PVR2_MAGIC = FOURCC('P', 'V', 'R', '!'); 
    const uint32 PVR3_MAGIC = FOURCC('P', 'V', 'R', 3); 

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
        uint32 headerLength;
        uint32 height;
        uint32 width;
        uint32 numMipmaps;
        uint32 flags;
        uint32 dataLength;
        uint32 bpp;
        uint32 bitmaskRed;
        uint32 bitmaskGreen;
        uint32 bitmaskBlue;
        uint32 bitmaskAlpha;
        uint32 pvrTag;
        uint32 numSurfs;
    } PVRTCTexHeaderV2;

    typedef struct _PVRTCTexHeaderV3
    {
        uint32  version;         //Version of the file header, used to identify it.
        uint32  flags;           //Various format flags.
        uint64  pixelFormat;     //The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
        uint32  colourSpace;     //The Colour Space of the texture, currently either linear RGB or sRGB.
        uint32  channelType;     //Variable type that the channel is stored in. Supports signed/unsigned int/short/byte or float for now.
        uint32  height;          //Height of the texture.
        uint32  width;           //Width of the texture.
        uint32  depth;           //Depth of the texture. (Z-slices)
        uint32  numSurfaces;     //Number of members in a Texture Array.
        uint32  numFaces;        //Number of faces in a Cube Map. Maybe be a value other than 6.
        uint32  mipMapCount;     //Number of MIP Maps in the texture - NB: Includes top level.
        uint32  metaDataSize;    //Size of the accompanying meta data.
    } PVRTCTexHeaderV3;

    typedef struct _PVRTCMetaData
    {
        uint32 DevFOURCC;
        uint32 u32Key;
        uint32 u32DataSize;
        uint8* Data;
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
	bool CPVRTCCodec::decode(const char* pszName, void* pMemory, uint32 nNumBytes, IN OUT ImageData& imgData) const
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
        uint32 flags = 0, formatFlags = 0;
        size_t numFaces = 1; // Assume one face until we know otherwise

        // Read the PVRTC header
        stream->Read(&header, sizeof(PVRTCTexHeaderV2));

        // Get format flags
        flags = header.flags;
        flipEndian((void *)flags, sizeof(uint32));
        formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;

        uint32 bitmaskAlpha = header.bitmaskAlpha;
        flipEndian((void *)bitmaskAlpha, sizeof(uint32));

        imgData.depth = 1;
        imgData.width = header.width;
        imgData.height = header.height;
        imgData.num_mipmaps = static_cast<uint16>(header.numMipmaps);

		switch(formatFlags)
		{
		case kPVRTextureFlagTypeRGBA_4444:
			imgData.format = PF_R4G4B4A4;
			break;
		case kPVRTextureFlagTypeRGBA_5551:
			imgData.format = PF_A1R5G5B5;
			break;
		case kPVRTextureFlagTypeRGBA_8888:
			imgData.format = PF_A8B8G8R8;
			break;
		case kPVRTextureFlagTypeRGB_565:
			imgData.format = PF_R5G6B5;
			break;
		case kPVRTextureFlagTypeRGB_888:
			imgData.format = PF_B8G8R8;
			break;
		case kPVRTextureFlagTypeI_8:
			imgData.format = PF_L8;
			break;
		case kPVRTextureFlagTypeAI_88:
			imgData.format = PF_BYTE_LA;
			break;
		case kPVRTextureFlagTypePVRTC_2:
			imgData.format = bitmaskAlpha ? PF_PVRTC_RGBA2 : PF_PVRTC_RGB2;
			imgData.flags |= IF_COMPRESSED;
			break;
		case kPVRTextureFlagTypePVRTC_4:
			imgData.format = bitmaskAlpha ? PF_PVRTC_RGBA4 : PF_PVRTC_RGB4;
			imgData.flags |= IF_COMPRESSED;
			break;
		case kPVRTextureFlagTypeBGRA_8888:
			imgData.format = PF_A8R8G8B8;
			break;
		case kPVRTextureFlagTypeA_8:
			imgData.format = PF_A8;
			break;
		case kPVRTextureFlagTypeETC:
			ASSERT(bitmaskAlpha == false);
			imgData.format = PF_ETC1_RGB8;
			imgData.flags |= IF_COMPRESSED;
			break;
		case kPVRTextureFlagTypeRGB_555://Unsupport
		default:
			LogError("UnSupport file: %s, formatFlags:%d", stream->GetName(), formatFlags);
			return false;
			break;
		}

        // Calculate total size from number of mipmaps, faces and size
		imgData.size = CImageCodec::calculateSize(imgData.num_mipmaps, numFaces, 
                                             imgData.width, imgData.height, imgData.depth, imgData.format);

		RefPtr<MemoryStream> output;
		// Bind output buffer
		output = CreateMemoryStream(imgData.size, false);

		// Now deal with the data
		void *destPtr = output->GetPtr();
        stream->Read(destPtr, imgData.size);
        destPtr = static_cast<void*>(static_cast<uint8*>(destPtr));

		imgData.memory = output;
		return true;
    }

    //---------------------------------------------------------------------  
	bool CPVRTCCodec::decodeV3(MemoryStream* stream, IN OUT ImageData& imgData) const
	{
		PVRTCTexHeaderV3 header;
		PVRTCMetadata metadata;
		uint32 flags = 0;
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
			imgData.format = PF_PVRTC_RGB2;
			break;
		case kPVRTC1_PF_2BPP_RGBA:
			imgData.format = PF_PVRTC_RGBA2;
			break;
		case kPVRTC1_PF_4BPP_RGB:
			imgData.format = PF_PVRTC_RGB4;
			break;
		case kPVRTC1_PF_4BPP_RGBA:
			imgData.format = PF_PVRTC_RGBA4;
			break;
		case kPVRTC2_PF_2BPP:
			imgData.format = PF_PVRTC2_2BPP;
			break;
		case kPVRTC2_PF_4BPP:
			imgData.format = PF_PVRTC2_4BPP;
			break;
		}

		// Get format flags
		flags = header.flags;
		flipEndian((void *)flags, sizeof(uint32));

		imgData.depth = header.depth;
		imgData.width = header.width;
		imgData.height = header.height;
		ASSERT(header.mipMapCount >= 1);
		imgData.num_mipmaps = static_cast<uint16>(header.mipMapCount-1);//v3格式要减去1，v2不需要

		// PVRTC is a compressed format
		imgData.flags |= IF_COMPRESSED;

		if(header.numFaces == 6)
			imgData.flags |= IF_CUBEMAP;

		if(header.depth > 1)
			imgData.flags |= IF_3D_TEXTURE;

		// Calculate total size from number of mipmaps, faces and size
		imgData.size = CImageCodec::calculateSize(imgData.num_mipmaps, numFaces, 
			imgData.width, imgData.height, imgData.depth, imgData.format);

		// Bind output buffer
		RefPtr<MemoryStream> output = CreateMemoryStream(imgData.size, false);

		// Now deal with the data
		void *destPtr = output->GetPtr();

		size_t width = imgData.width;
		size_t height = imgData.height;
		size_t depth = imgData.depth;

		// All mips for a surface, then each face
		for(size_t mip = 0; mip <= imgData.num_mipmaps; ++mip)
		{
			for(size_t surface = 0; surface < header.numSurfaces; ++surface)
			{
				for(size_t i = 0; i < numFaces; ++i)
				{
					// Load directly
					size_t pvrSize = PixelUtil::getMemorySize(width, height, depth, imgData.format);
					stream->Read(destPtr, pvrSize);
					destPtr = static_cast<void*>(static_cast<uint8*>(destPtr) + pvrSize);
				}
			}

			// Next mip
			if(width!=1) width /= 2;
			if(height!=1) height /= 2;
			if(depth!=1) depth /= 2;
		}

		imgData.memory = output;
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
		if (maxbytes >= sizeof(uint32))
		{
			uint32 fileType;
			memcpy(&fileType, magicNumberPtr, sizeof(uint32));
			flipEndian(&fileType, sizeof(uint32), 1);

			if (PVR3_MAGIC == fileType || PVR2_MAGIC == fileType)
			{
				return string("pvr");
			}
		}

		return StringUtil::BLANK;
	}
}
