#include "DDSCodec.h"
#include "PixelFormat.h"
#include "Engine/RenderSystem/DeviceCapabilities.h"
 
namespace ma
{
	// Internal DDS structure definitions
#define FOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))
	
#ifdef WIN32
#pragma pack (push, 1)
#else
#pragma pack (1)
#endif

	// Nested structure
	struct DDSPixelFormat
	{
		uint32_t size;
		uint32_t flags;
		uint32_t fourCC;
		uint32_t rgbBits;
		uint32_t redMask;
		uint32_t greenMask;
		uint32_t blueMask;
		uint32_t alphaMask;
	};
	
	// Nested structure
	struct DDSCaps
	{
		uint32_t caps1;
		uint32_t caps2;
		uint32_t reserved[2];
	};
	// Main header, note preceded by 'DDS '
	struct DDSHeader
	{
		uint32_t size;		
		uint32_t flags;
		uint32_t height;
		uint32_t width;
		uint32_t sizeOrPitch;
		uint32_t depth;
		uint32_t mipMapCount;
		uint32_t reserved1[11];
		DDSPixelFormat pixelFormat;
		DDSCaps caps;
		uint32_t reserved2;
	};

	// An 8-byte DXT colour block, represents a 4x4 texel area. Used by all DXT formats
	struct DXTColourBlock
	{
		// 2 colour ranges
		uint16_t colour_0;
		uint16_t colour_1;
		// 16 2-bit indexes, each byte here is one row
		uint8_t indexRow[4];
	};
	// An 8-byte DXT explicit alpha block, represents a 4x4 texel area. Used by DXT2/3
	struct DXTExplicitAlphaBlock
	{
		// 16 4-bit values, each 16-bit value is one row
		uint16_t alphaRow[4];
	};
	// An 8-byte DXT interpolated alpha block, represents a 4x4 texel area. Used by DXT4/5
	struct DXTInterpolatedAlphaBlock
	{
		// 2 alpha ranges
		uint8_t alpha_0;
		uint8_t alpha_1;
		// 16 3-bit indexes. Unfortunately 3 bits doesn't map too well to row bytes
		// so just stored raw
		uint8_t indexes[6];
	};
	
#ifdef WIN32
#pragma pack (pop)
#else
#pragma pack ()
#endif

	const uint32_t DDS_MAGIC = FOURCC('D', 'D', 'S', ' ');
	const uint32_t DDS_PIXELFORMAT_SIZE = 8 * sizeof(uint32_t);
	const uint32_t DDS_CAPS_SIZE = 4 * sizeof(uint32_t);
	const uint32_t DDS_HEADER_SIZE = 19 * sizeof(uint32_t) + DDS_PIXELFORMAT_SIZE + DDS_CAPS_SIZE;

	const uint32_t DDSD_CAPS = 0x00000001;
	const uint32_t DDSD_HEIGHT = 0x00000002;
	const uint32_t DDSD_WIDTH = 0x00000004;
	const uint32_t DDSD_PITCH = 0x00000008;
	const uint32_t DDSD_PIXELFORMAT = 0x00001000;
	const uint32_t DDSD_MIPMAPCOUNT = 0x00020000;
	const uint32_t DDSD_LINEARSIZE = 0x00080000;
	const uint32_t DDSD_DEPTH = 0x00800000;
	const uint32_t DDPF_ALPHAPIXELS = 0x00000001;
	const uint32_t DDPF_FOURCC = 0x00000004;
	const uint32_t DDPF_RGB = 0x00000040;
	const uint32_t DDSCAPS_COMPLEX = 0x00000008;
	const uint32_t DDSCAPS_TEXTURE = 0x00001000;
	const uint32_t DDSCAPS_MIPMAP = 0x00400000;
	const uint32_t DDSCAPS2_CUBEMAP = 0x00000200;
	const uint32_t DDSCAPS2_CUBEMAP_POSITIVEX = 0x00000400;
	const uint32_t DDSCAPS2_CUBEMAP_NEGATIVEX = 0x00000800;
	const uint32_t DDSCAPS2_CUBEMAP_POSITIVEY = 0x00001000;
	const uint32_t DDSCAPS2_CUBEMAP_NEGATIVEY = 0x00002000;
	const uint32_t DDSCAPS2_CUBEMAP_POSITIVEZ = 0x00004000;
	const uint32_t DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x00008000;
	const uint32_t DDSCAPS2_VOLUME = 0x00200000;

	// Special FourCC codes
	const uint32_t D3DFMT_R16F			= 111;
	const uint32_t D3DFMT_G16R16F			= 112;
	const uint32_t D3DFMT_A16B16G16R16F	= 113;
	const uint32_t D3DFMT_R32F            = 114;
	const uint32_t D3DFMT_G32R32F         = 115;
	const uint32_t D3DFMT_A32B32G32R32F   = 116;

	//---------------------------------------------------------------------
    CDDSCodec::CDDSCodec()
    { 
    }
    //---------------------------------------------------------------------
    /*DataStreamPtr DDSCodec::code(MemoryDataStreamPtr& input, Codec::CodecDataPtr& pData) const
    {        
		OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
			"DDS encoding not supported",
			"DDSCodec::code" ) ;
    }
    //---------------------------------------------------------------------
    void DDSCodec::codeToFile(MemoryDataStreamPtr& input, 
        const string& outFileName, Codec::CodecDataPtr& pData) const
    {
		// Unwrap codecDataPtr - data is cleaned by calling function
		ImageData* imgData = static_cast<ImageData* >(pData.getPointer());  


		// Check size for cube map faces
		bool isCubeMap = (imgData.size == 
			Image::calculateSize(imgData.num_mipmaps, 6, imgData.width, 
			imgData.height, imgData.depth, imgData.format));

		// Establish texture attributes
		bool isVolume = (imgData.depth > 1);		
		bool isFloat32r = (imgData.format == PF_FLOAT32_R);
		bool hasAlpha = false;
		bool notImplemented = false;
		string notImplementedString = "";

		// Check for all the 'not implemented' conditions
		if (imgData.num_mipmaps != 0)
		{
			// No mip map functionality yet
			notImplemented = true;
			notImplementedString += " mipmaps";
		}

		if ((isVolume == true)&&(imgData.width != imgData.height))
		{
			// Square textures only
			notImplemented = true;
			notImplementedString += " non square textures";
		}

		uint32_t size = 1;
		while (size < imgData.width)
		{
			size <<= 1;
		}
		if (size != imgData.width)
		{
			// Power two textures only
			notImplemented = true;
			notImplementedString += " non power two textures";
		}

		switch(imgData.format)
		{
		case PF_A8R8G8B8:
		case PF_X8R8G8B8:
		case PF_R8G8B8:
		case PF_FLOAT32_R:
			break;
		default:
			// No crazy FOURCC or 565 et al. file formats at this stage
			notImplemented = true;
			notImplementedString = " unsupported pixel format";
			break;
		}		



		// Except if any 'not implemented' conditions were met
		if (notImplemented)
		{
			OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
				"DDS encoding for" + notImplementedString + " not supported",
				"DDSCodec::codeToFile" ) ;
		}
		else
		{
			// Build header and write to disk

			// Variables for some DDS header flags
			uint32_t ddsHeaderFlags = 0;			
			uint32_t ddsHeaderRgbBits = 0;
			uint32_t ddsHeaderSizeOrPitch = 0;
			uint32_t ddsHeaderCaps1 = 0;
			uint32_t ddsHeaderCaps2 = 0;
			uint32_t ddsMagic = DDS_MAGIC;

			// Initalise the header flags
			ddsHeaderFlags = (isVolume) ? DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT|DDSD_DEPTH|DDSD_PIXELFORMAT :
				DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT|DDSD_PIXELFORMAT;	

			// Initalise the rgbBits flags
			switch(imgData.format)
			{
			case PF_A8R8G8B8:
				ddsHeaderRgbBits = 8 * 4;
				hasAlpha = true;
				break;
			case PF_X8R8G8B8:
				ddsHeaderRgbBits = 8 * 4;
				break;
			case PF_R8G8B8:
				ddsHeaderRgbBits = 8 * 3;
				break;
			case PF_FLOAT32_R:
				ddsHeaderRgbBits = 32;
				break;
			default:
				ddsHeaderRgbBits = 0;
				break;
			}

			// Initalise the SizeOrPitch flags (power two textures for now)
			ddsHeaderSizeOrPitch = ddsHeaderRgbBits * imgData.width;

			// Initalise the caps flags
			ddsHeaderCaps1 = (isVolume||isCubeMap) ? DDSCAPS_COMPLEX|DDSCAPS_TEXTURE : DDSCAPS_TEXTURE;
			if (isVolume)
			{
				ddsHeaderCaps2 = DDSCAPS2_VOLUME;
			}
			else if (isCubeMap)
			{
				ddsHeaderCaps2 = DDSCAPS2_CUBEMAP|
					DDSCAPS2_CUBEMAP_POSITIVEX|DDSCAPS2_CUBEMAP_NEGATIVEX|
					DDSCAPS2_CUBEMAP_POSITIVEY|DDSCAPS2_CUBEMAP_NEGATIVEY|
					DDSCAPS2_CUBEMAP_POSITIVEZ|DDSCAPS2_CUBEMAP_NEGATIVEZ;
			}

			// Populate the DDS header information
			DDSHeader ddsHeader;
			ddsHeader.size = DDS_HEADER_SIZE;
			ddsHeader.flags = ddsHeaderFlags;		
			ddsHeader.width = (uint32_t)imgData.width;
			ddsHeader.height = (uint32_t)imgData.height;
			ddsHeader.depth = (uint32_t)(isVolume ? imgData.depth : 0);
			ddsHeader.depth = (uint32_t)(isCubeMap ? 6 : ddsHeader.depth);
			ddsHeader.mipMapCount = 0;
			ddsHeader.sizeOrPitch = ddsHeaderSizeOrPitch;
			for (uint32_t reserved1=0; reserved1<11; reserved1++) // XXX nasty constant 11
			{
				ddsHeader.reserved1[reserved1] = 0;
			}
			ddsHeader.reserved2 = 0;

			ddsHeader.pixelFormat.size = DDS_PIXELFORMAT_SIZE;
			ddsHeader.pixelFormat.flags = (hasAlpha) ? DDPF_RGB|DDPF_ALPHAPIXELS : DDPF_RGB;
			ddsHeader.pixelFormat.flags = (isFloat32r) ? DDPF_FOURCC : ddsHeader.pixelFormat.flags;
			ddsHeader.pixelFormat.fourCC = (isFloat32r) ? D3DFMT_R32F : 0;
			ddsHeader.pixelFormat.rgbBits = ddsHeaderRgbBits;

			ddsHeader.pixelFormat.alphaMask = (hasAlpha)   ? 0xFF000000 : 0x00000000;
			ddsHeader.pixelFormat.alphaMask = (isFloat32r) ? 0x00000000 : ddsHeader.pixelFormat.alphaMask;
			ddsHeader.pixelFormat.redMask   = (isFloat32r) ? 0xFFFFFFFF :0x00FF0000;
			ddsHeader.pixelFormat.greenMask = (isFloat32r) ? 0x00000000 :0x0000FF00;
			ddsHeader.pixelFormat.blueMask  = (isFloat32r) ? 0x00000000 :0x000000FF;

			ddsHeader.caps.caps1 = ddsHeaderCaps1;
			ddsHeader.caps.caps2 = ddsHeaderCaps2;
			ddsHeader.caps.reserved[0] = 0;
			ddsHeader.caps.reserved[1] = 0;

			// Swap endian
			flipEndian(&ddsMagic, sizeof(uint32_t), 1);
			flipEndian(&ddsHeader, 4, sizeof(DDSHeader) / 4);

			// Write the file 			
			std::ofstream of;
			of.open(outFileName.c_str(), std::ios_base::binary|std::ios_base::out);
			of.write((const char *)&ddsMagic, sizeof(uint32_t));
			of.write((const char *)&ddsHeader, DDS_HEADER_SIZE);
			// XXX flipEndian on each pixel chunk written unless isFloat32r ?
			of.write((const char *)input->getPtr(), (uint32_t)imgData.size);
			of.close();
		}
	}*/
	//---------------------------------------------------------------------
	PixelFormat CDDSCodec::convertFourCCFormat(uint32_t fourcc) const
	{
		// convert dxt pixel format
		switch(fourcc)
		{
		case FOURCC('D','X','T','1'):
			return PF_DXT1;
		case FOURCC('D','X','T','2'):
			return PF_DXT2;
		case FOURCC('D','X','T','3'):
			return PF_DXT3;
		case FOURCC('D','X','T','4'):
			return PF_DXT4;
		case FOURCC('D','X','T','5'):
			return PF_DXT5;
		case D3DFMT_R16F:
			return PF_FLOAT16_R;
		case D3DFMT_G16R16F:
			return PF_FLOAT16_GR;
		case D3DFMT_A16B16G16R16F:
			return PF_FLOAT16_RGBA;
		case D3DFMT_R32F:
			return PF_FLOAT32_R;
		case D3DFMT_G32R32F:
			return PF_FLOAT32_GR;
		case D3DFMT_A32B32G32R32F:
			return PF_FLOAT32_RGBA;
		// We could support 3Dc here, but only ATI cards support it, not nVidia
		default:
			LogError("Unsupported FourCC format found in DDS file, DDSCodec::decode");
		};

		return PF_UNKNOWN;
	}
	//---------------------------------------------------------------------
	PixelFormat CDDSCodec::convertPixelFormat(uint32_t rgbBits, uint32_t rMask, 
		uint32_t gMask, uint32_t bMask, uint32_t aMask) const
	{
		// General search through pixel formats
		for (int i = PF_UNKNOWN + 1; i < PF_COUNT; ++i)
		{
			PixelFormat pf = static_cast<PixelFormat>(i);
			if (PixelUtil::getNumElemBits(pf) == rgbBits)
			{
				uint64_t testMasks[4];
				PixelUtil::getBitMasks(pf, testMasks);
				int testBits[4];
				PixelUtil::getBitDepths(pf, testBits);
				if (testMasks[0] == rMask && testMasks[1] == gMask &&
					testMasks[2] == bMask && 
					// for alpha, deal with 'X8' formats by checking bit counts
					(testMasks[3] == aMask || (aMask == 0 && testBits[3] == 0)))
				{
					return pf;
				}
			}

		}

		LogError("%s%s", "Cannot determine pixel format",
			"DDSCodec::convertPixelFormat");
		return PF_UNKNOWN;

	}
	//---------------------------------------------------------------------
	void CDDSCodec::unpackDXTColour(PixelFormat pf, const DXTColourBlock& block, 
		ColourValue* pCol) const
	{
		// Note - we assume all values have already been endian swapped

		// Colour lookup table
		ColourValue derivedColours[4];

		if (pf == PF_DXT1 && block.colour_0 <= block.colour_1)
		{
			// 1-bit alpha
			PixelUtil::unpackColour(&(derivedColours[0]), PF_R5G6B5, &(block.colour_0));
			PixelUtil::unpackColour(&(derivedColours[1]), PF_R5G6B5, &(block.colour_1));
			// one intermediate colour, half way between the other two
			derivedColours[2] = (derivedColours[0] + derivedColours[1]) / 2;
			// transparent colour
			derivedColours[3] = ColourValue::ZERO;
		}
		else
		{
			PixelUtil::unpackColour(&(derivedColours[0]), PF_R5G6B5, &(block.colour_0));
			PixelUtil::unpackColour(&(derivedColours[1]), PF_R5G6B5, &(block.colour_1));
			// first interpolated colour, 1/3 of the way along
			derivedColours[2] = (2 * derivedColours[0] + derivedColours[1]) / 3;
			// second interpolated colour, 2/3 of the way along
			derivedColours[3] = (derivedColours[0] + 2 * derivedColours[1]) / 3;
		}

		// Process 4x4 block of texels
		for (uint32_t row = 0; row < 4; ++row)
		{
			for (uint32_t x = 0; x < 4; ++x)
			{
				// LSB come first
				uint8_t colIdx = static_cast<uint8_t>(block.indexRow[row] >> (x * 2) & 0x3);
				if (pf == PF_DXT1)
				{
					// Overwrite entire colour
					pCol[(row * 4) + x] = derivedColours[colIdx];
				}
				else
				{
					// alpha has already been read (alpha precedes colour)
					ColourValue& col = pCol[(row * 4) + x];
					col.r = derivedColours[colIdx].r;
					col.g = derivedColours[colIdx].g;
					col.b = derivedColours[colIdx].b;
				}
			}

		}


	}
	//---------------------------------------------------------------------
	void CDDSCodec::unpackDXTAlpha(
		const DXTExplicitAlphaBlock& block, ColourValue* pCol) const
	{
		// Note - we assume all values have already been endian swapped
		
		// This is an explicit alpha block, 4 bits per pixel, LSB first
		for (uint32_t row = 0; row < 4; ++row)
		{
			for (uint32_t x = 0; x < 4; ++x)
			{
				// Shift and mask off to 4 bits
				uint8_t val = static_cast<uint8_t>(block.alphaRow[row] >> (x * 4) & 0xF);
				// Convert to [0,1]
				pCol->a = (float)val / (float)0xF;
				pCol++;
				
			}
			
		}

	}
	//---------------------------------------------------------------------
	void CDDSCodec::unpackDXTAlpha(
		const DXTInterpolatedAlphaBlock& block, ColourValue* pCol) const
	{
		// 8 derived alpha values to be indexed
		float derivedAlphas[8];

		// Explicit extremes
		derivedAlphas[0] = block.alpha_0 / (float)0xFF;
		derivedAlphas[1] = block.alpha_1 / (float)0xFF;
		
		
		if (block.alpha_0 <= block.alpha_1)
		{
			// 4 interpolated alphas, plus zero and one			
			// full range including extremes at [0] and [5]
			// we want to fill in [1] through [4] at weights ranging
			// from 1/5 to 4/5
			float denom = 1.0f / 5.0f;
			for (uint32_t i = 0; i < 4; ++i) 
			{
				float factor0 = (4 - i) * denom;
				float factor1 = (i + 1) * denom;
				derivedAlphas[i + 2] = 
					(factor0 * block.alpha_0) + (factor1 * block.alpha_1);
			}
			derivedAlphas[6] = 0.0f;
			derivedAlphas[7] = 1.0f;

		}
		else
		{
			// 6 interpolated alphas
			// full range including extremes at [0] and [7]
			// we want to fill in [1] through [6] at weights ranging
			// from 1/7 to 6/7
			float denom = 1.0f / 7.0f;
			for (uint32_t i = 0; i < 6; ++i) 
			{
				float factor0 = (6 - i) * denom;
				float factor1 = (i + 1) * denom;
				derivedAlphas[i + 2] = 
					(factor0 * block.alpha_0) + (factor1 * block.alpha_1);
			}
			
		}

		// Ok, now we've built the reference values, process the indexes
		for (uint32_t i = 0; i < 16; ++i)
		{
			uint32_t baseByte = (i * 3) / 8;
			uint32_t baseBit = (i * 3) % 8;
			uint8_t bits = static_cast<uint8_t>(block.indexes[baseByte] >> baseBit & 0x7);
			// do we need to stitch in next byte too?
			if (baseBit > 5)
			{
				uint8_t extraBits = static_cast<uint8_t>(
					(block.indexes[baseByte+1] << (8 - baseBit)) & 0xFF);
				bits |= extraBits & 0x7;
			}
			pCol[i].a = derivedAlphas[bits];

		}

	}
    //---------------------------------------------------------------------
    //Codec::DecodeResult DDSCodec::decode(DataStreamPtr& stream) const
	bool CDDSCodec::decode(const char* pszName, void* pMemory, uint32_t nNumBytes, IN OUT ImageData& imgData) const
    {
		MemoryStream stream(pszName, pMemory, nNumBytes, true);

		// Read 4 character code
		uint32_t fileType;
		stream.Read(&fileType, sizeof(uint32_t));
		flipEndian(&fileType, sizeof(uint32_t), 1);
		
		if (FOURCC('D', 'D', 'S', ' ') != fileType)
		{
			LogError("%s%s",
				"This is not a DDS file!", "DDSCodec::decode");
			return false;
		}
		
		// Read header in full
		DDSHeader header;
		stream.Read(&header, sizeof(DDSHeader));

		// Endian flip if required, all 32-bit values
		flipEndian(&header, 4, sizeof(DDSHeader) / 4);

		// Check some sizes
		if (header.size != DDS_HEADER_SIZE)
		{
			LogError("%s%s",
				"DDS header size mismatch!", "DDSCodec::decode");
			return false;
		}
		if (header.pixelFormat.size != DDS_PIXELFORMAT_SIZE)
		{
			LogError("%s%s",
				"DDS header size mismatch!", "DDSCodec::decode");
			return false;
		}

		RefPtr<MemoryStream> output;

		imgData.m_nDepth = 1; // (deal with volume later)
		imgData.m_nWidth = header.width;
		imgData.m_nHeight = header.height;
		uint32_t numFaces = 1; // assume one face until we know otherwise

		if (header.caps.caps1 & DDSCAPS_MIPMAP)
		{
	        imgData.m_nNumMipmaps = static_cast<unsigned short>(header.mipMapCount - 1);
		}
		else
		{
			imgData.m_nNumMipmaps = 0;
		}
		imgData.m_nFlags = 0;

		bool decompressDXT = false;
		// Figure out basic image type
		if (header.caps.caps2 & DDSCAPS2_CUBEMAP)
		{
			imgData.m_nFlags |= IF_CUBEMAP;
			numFaces = 6;
		}
		else if (header.caps.caps2 & DDSCAPS2_VOLUME)
		{
			imgData.m_nFlags |= IF_3D_TEXTURE;
			imgData.m_nDepth = header.depth;
		}
		// Pixel format
		PixelFormat sourceFormat = PF_UNKNOWN;

		if (header.pixelFormat.flags & DDPF_FOURCC)
		{
			sourceFormat = convertFourCCFormat(header.pixelFormat.fourCC);
		}
		else
		{
			sourceFormat = convertPixelFormat(header.pixelFormat.rgbBits, 
				header.pixelFormat.redMask, header.pixelFormat.greenMask, 
				header.pixelFormat.blueMask, 
				header.pixelFormat.flags & DDPF_ALPHAPIXELS ? 
				header.pixelFormat.alphaMask : 0);
		}

		if (PixelUtil::isCompressed(sourceFormat))
		{
			if (!GetDeviceCapabilities()->GetTextureDXTSupported())
			{
				// We'll need to decompress
				decompressDXT = true;
				// Convert format
				switch (sourceFormat)
				{
				case PF_DXT1:
					// source can be either 565 or 5551 depending on whether alpha present
					// unfortunately you have to read a block to figure out which
					// Note that we upgrade to 32-bit pixel formats here, even 
					// though the source is 16-bit; this is because the interpolated
					// values will benefit from the 32-bit results, and the source
					// from which the 16-bit samples are calculated may have been
					// 32-bit so can benefit from this.
					DXTColourBlock block;
					stream.Read(&block, sizeof(DXTColourBlock));
					flipEndian(&(block.colour_0), sizeof(uint16_t), 1);
					flipEndian(&(block.colour_1), sizeof(uint16_t), 1);
					// skip back since we'll need to read this again
					stream.Skip(0 - (long)sizeof(DXTColourBlock));
					// colour_0 <= colour_1 means transparency in DXT1
					if (block.colour_0 <= block.colour_1)
					{
						imgData.m_eFormat = PF_BYTE_RGBA;
					}
					else
					{
						imgData.m_eFormat = PF_BYTE_RGB;
					}
					break;
				case PF_DXT2:
				case PF_DXT3:
				case PF_DXT4:
				case PF_DXT5:
					// full alpha present, formats vary only in encoding 
					imgData.m_eFormat = PF_BYTE_RGBA;
					break;
                default:
                    // all other cases need no special format handling
                    break;
				}
			}
			else
			{
				// Use original format
				imgData.m_eFormat = sourceFormat;
				// Keep DXT data compressed
				imgData.m_nFlags |= IF_COMPRESSED;
			}
		}
		else // not compressed
		{
			// Don't test against DDPF_RGB since greyscale DDS doesn't set this
			// just derive any other kind of format
			imgData.m_eFormat = sourceFormat;
		}

		// Calculate total size from number of mipmaps, faces and size
		imgData.m_nSize = CImageCodec::calculateSize(imgData.m_nNumMipmaps, numFaces, 
			imgData.m_nWidth, imgData.m_nHeight, imgData.m_nDepth, imgData.m_eFormat);

		// Bind output buffer
		output = CreateMemoryStream(imgData.m_nSize, false);

		
		// Now deal with the data
		void* destPtr = output->GetPtr();

		// all mips for a face, then each face
		for ( uint32_t i = 0; i < numFaces; ++i )
		{   
			uint32_t width = imgData.m_nWidth;
			uint32_t height = imgData.m_nHeight;
			uint32_t depth = imgData.m_nDepth;
			for ( uint32_t mip=0; mip<=imgData.m_nNumMipmaps; ++mip )
			{
				uint32_t dstPitch = width * PixelUtil::getNumElemBytes( imgData.m_eFormat );
				if ( PixelUtil::isCompressed(sourceFormat) )
				{
					// Compressed data
					if ( decompressDXT )
					{
						DXTColourBlock col;
						DXTInterpolatedAlphaBlock iAlpha;
						DXTExplicitAlphaBlock eAlpha;
						// 4x4 block of decompressed colour
						ColourValue tempColours[16];
						uint32_t destBpp = PixelUtil::getNumElemBytes(imgData.m_eFormat);
						uint32_t sx = min(width, (uint32_t)4);
						uint32_t sy = min(height, (uint32_t)4);
						uint32_t destPitchMinus4 = dstPitch - destBpp * sx;
						// slices are done individually
						for(uint32_t z = 0; z < depth; ++z)
						{
							// 4x4 blocks in x/y
							for (uint32_t y = 0; y < height; y += 4)
							{
								for (uint32_t x = 0; x < width; x += 4)
								{
									if (sourceFormat == PF_DXT2 || 
										sourceFormat == PF_DXT3)
									{
										// explicit alpha
										stream.Read(&eAlpha, sizeof(DXTExplicitAlphaBlock));
										flipEndian(eAlpha.alphaRow, sizeof(uint16_t), 4);
										unpackDXTAlpha(eAlpha, tempColours) ;
									}
									else if (sourceFormat == PF_DXT4 || 
										sourceFormat == PF_DXT5)
									{
										// interpolated alpha
										stream.Read(&iAlpha, sizeof(DXTInterpolatedAlphaBlock));
										flipEndian(&(iAlpha.alpha_0), sizeof(uint16_t), 1);
										flipEndian(&(iAlpha.alpha_1), sizeof(uint16_t), 1);
										unpackDXTAlpha(iAlpha, tempColours) ;
									}
									// always read colour
									stream.Read(&col, sizeof(DXTColourBlock));
									flipEndian(&(col.colour_0), sizeof(uint16_t), 1);
									flipEndian(&(col.colour_1), sizeof(uint16_t), 1);
									unpackDXTColour(sourceFormat, col, tempColours);

									// write 4x4 block to uncompressed version
									for (uint32_t by = 0; by < sy; ++by)
									{
										for (uint32_t bx = 0; bx < sx; ++bx)
										{
											PixelUtil::packColour(tempColours[by*4+bx],
												imgData.m_eFormat, destPtr);
											destPtr = static_cast<void*>(
												static_cast<uint8_t*>(destPtr) + destBpp);
										}
										// advance to next row
										destPtr = static_cast<void*>(
											static_cast<uint8_t*>(destPtr) + destPitchMinus4);
									}
									// next block. Our dest pointer is 4 lines down
									// from where it started
									if (x + 4 >= width)
									{
										// Jump back to the start of the line
										destPtr = static_cast<void*>(
											static_cast<uint8_t*>(destPtr) - destPitchMinus4);
									}
									else
									{
										// Jump back up 4 rows and 4 pixels to the
										// right to be at the next block to the right
										destPtr = static_cast<void*>(
											static_cast<uint8_t*>(destPtr) - dstPitch * sy + destBpp * sx);

									}

								}

							}
						}

					}
					else
					{
						// load directly
						// DDS format lies! sizeOrPitch is not always set for DXT!!
						uint32_t dxtSize = PixelUtil::getMemorySize( width, height, depth, imgData.m_eFormat );
						stream.Read( destPtr, dxtSize );
						destPtr = static_cast<void*>( static_cast<uint8_t*>(destPtr) + dxtSize );
					}

				}
				else
				{
					// Final data - trim incoming pitch
					uint32_t srcPitch;
					if ( header.flags & DDSD_PITCH )
					{
                        srcPitch = header.sizeOrPitch / max<uint32_t>(1,mip * 2);
					}
					else
					{
						// assume same as final pitch
						srcPitch = dstPitch;
					}
					ASSERT ( dstPitch <= srcPitch );
					long srcAdvance = static_cast<long>(srcPitch) - static_cast<long>(dstPitch);
					for ( uint32_t z=0; z<depth; ++z )
					{
						for ( uint32_t y = 0; y < height; ++y )
						{
							stream.Read(destPtr, (uint32_t)dstPitch);
							if (srcAdvance > 0)
								stream.Skip(srcAdvance);
							destPtr = static_cast<void*>(static_cast<uint8_t*>(destPtr) + dstPitch);
						}
					}

				}

				
				/// Next mip
				if(width!=1) width /= 2;
				if(height!=1) height /= 2;
				if(depth!=1) depth /= 2;
			}

		}

// 		DecodeResult ret;
// 		ret.first = output;
// 		ret.second = CodecDataPtr(imgData);
		imgData.m_pMemory = output;
		return true;
    }
    //---------------------------------------------------------------------    
    void CDDSCodec::flipEndian(void * pData, uint32_t size, uint32_t count) const
    {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
		for(unsigned int index = 0; index < count; index++)
        {
            flipEndian((void *)((long)pData + (index * size)), size);
        }
#endif
    }
    //---------------------------------------------------------------------    
    void CDDSCodec::flipEndian(void * pData, uint32_t size) const
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
	string CDDSCodec::magicNumberToFileExt(const char *magicNumberPtr, uint32_t maxbytes) const
	{
		if (maxbytes >= sizeof(uint32_t))
		{
			uint32_t fileType;
			memcpy(&fileType, magicNumberPtr, sizeof(uint32_t));
			flipEndian(&fileType, sizeof(uint32_t), 1);

			if (DDS_MAGIC == fileType)
			{
				return string("dds");
			}
		}

		return StringUtil::BLANK;

	}
	
}

