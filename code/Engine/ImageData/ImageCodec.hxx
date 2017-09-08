#include "ImageCodec.h"
#include "ImageCodecResampler.h"
#include "PixelFormat.h"


#define FREEIMAGE_LIB
#include "FreeImage.h"

// freeimage 3.9.1~3.11.0 interoperability fix
#ifndef FREEIMAGE_COLORORDER
	// we have freeimage 3.9.1, define these symbols in such way as 3.9.1 really work (do not use 3.11.0 definition, as color order was changed between these two versions on Apple systems)
	#define FREEIMAGE_COLORORDER_BGR	0
	#define FREEIMAGE_COLORORDER_RGB	1
	#if defined(FREEIMAGE_BIGENDIAN)
		#define FREEIMAGE_COLORORDER FREEIMAGE_COLORORDER_RGB
	#else
		#define FREEIMAGE_COLORORDER FREEIMAGE_COLORORDER_BGR
	#endif
#endif

// Color order is actually RGB on iPhone
//#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	#undef FREEIMAGE_COLORORDER
//	#define FREEIMAGE_COLORORDER FREEIMAGE_COLORORDER_RGB
//#endif

namespace ma
{

CImageCodec::CImageCodec(void)
{
	m_nWidth = m_nHeight = 0;
}

CImageCodec::~CImageCodec(void)
{
}

bool CImageCodec::Load(const char* pszFile)
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(pszFile);
	if(fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(pszFile);
		if (fif == FIF_UNKNOWN)
		{
			LogError(
				"Error getformat %s", pszFile);
			return false;
		}
	}

	FIBITMAP* pBitmap = NULL;
	if (FreeImage_FIFSupportsReading(fif))
	{
		pBitmap = FreeImage_Load(fif, pszFile);
	}

	if (pBitmap == NULL)
	{
		LogError(
			"Error FreeImage_Load file %s", pszFile);
		return false;
	}

	int nPitch = FreeImage_GetPitch(pBitmap);
	uint8* pBits = FreeImage_GetBits(pBitmap);
	int bpp = FreeImage_GetBPP(pBitmap);
	m_nWidth = FreeImage_GetWidth(pBitmap);
	m_nHeight = FreeImage_GetHeight(pBitmap);

	//pBitmap32= FreeImage_ConvertTo32Bits(pBitmap);
	switch (bpp)
	{
	case 8:
		{
			uint8* pCur = pBits;
			for (uint32 i = 0;i< m_nHeight;++i)
			{
				for (uint32 j = 0;j< m_nWidth;++j)
				{
					uint8 nValue = 0;
					memcpy(&nValue, (void*)(pCur+j*1), 1);
				}
				pCur += nPitch;
			}
		}
		break;
	case 16:
		{
			uint8* pCur = pBits;
			for (uint32 i = 0;i< m_nHeight;++i)
			{
				for (uint32 j = 0;j< m_nWidth;++j)
				{
					uint16 nValue = 0;
					memcpy(&nValue, (void*)(pCur+j*2), 2);
				}
				pCur += nPitch;
			}
		}
		break;
	case 24:
		{
			uint8* pCur = pBits;
			for (uint32 i = 0;i< m_nHeight;++i)
			{
				for (uint32 j = 0;j< m_nWidth;++j)
				{
					uint32 nValue = 0;
					memcpy(&nValue, (void*)(pCur+j*3), 3);
				}
				pCur += nPitch;
			}
		}
		break;
	case 32:
		{
			uint8* pCur = pBits;
			for (uint32 i = 0;i< m_nHeight;++i)
			{
				for (uint32 j = 0;j< m_nWidth;++j)
				{
					uint32 nValue = 0;
					memcpy(&nValue, (void*)(pCur+j*4), 4);
				}
				pCur += nPitch;
			}
		}
		break;
	default:
		LogError(
			"Error FreeImage_Load file %s", pszFile);
		return false;
	}

	FreeImage_Unload(pBitmap);
	return true;
}

// ---------------------------------------------------------------------
// Error handler
// ---------------------------------------------------------------------
void FreeImageLoadErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) 
{
	// Callback method as required by FreeImage to report problems
	const char* typeName = FreeImage_GetFormatFromFIF(fif);
	if (typeName)
	{
		LogError(
			"FreeImage error: ' %s ' when loading format %s",
			message,typeName);
	}
	else
	{
		LogError(
			"FreeImage error: ' %s '",
			message);
	}
}

// ---------------------------------------------------------------------
// Static Function
// ---------------------------------------------------------------------
void CImageCodec::Startup()
{
	// freeimage init
	FreeImage_Initialise(false);

	// Set error handler
	FreeImage_SetOutputMessage(FreeImageLoadErrorHandler);
}

void CImageCodec::Shutdown()
{
	FreeImage_DeInitialise();
}


//-----------------------------------------------------------------------
void CImageCodec::scale(const PixelBox &src, const PixelBox &scaled, Filter filter) 
{
	ASSERT(PixelUtil::isAccessible(src.format));
	ASSERT(PixelUtil::isAccessible(scaled.format));
	RefPtr<MemoryStream> buf; // For auto-delete
	PixelBox temp;
	switch (filter) 
	{
	default:
	case FILTER_NEAREST:
		if(src.format == scaled.format) 
		{
			// No intermediate buffer needed
			temp = scaled;
		}
		else
		{
			// Allocate temporary buffer of destination size in source format 
			temp = PixelBox(scaled.getWidth(), scaled.getHeight(), scaled.getDepth(), src.format);
			buf = CreateMemoryStream(temp.getConsecutiveSize(), false);
			temp.data = buf->GetPtr();
		}
		// super-optimized: no conversion
		switch (PixelUtil::getNumElemBytes(src.format)) 
		{
		case 1: NearestResampler<1>::scale(src, temp); break;
		case 2: NearestResampler<2>::scale(src, temp); break;
		case 3: NearestResampler<3>::scale(src, temp); break;
		case 4: NearestResampler<4>::scale(src, temp); break;
		case 6: NearestResampler<6>::scale(src, temp); break;
		case 8: NearestResampler<8>::scale(src, temp); break;
		case 12: NearestResampler<12>::scale(src, temp); break;
		case 16: NearestResampler<16>::scale(src, temp); break;
		default:
			// never reached
			ASSERT(false);
		}
		if(temp.data != scaled.data)
		{
			// Blit temp buffer
			PixelUtil::bulkPixelConversion(temp, scaled);
		}
		break;

	case FILTER_LINEAR:
	case FILTER_BILINEAR:
		switch (src.format) 
		{
		case PF_L8: case PF_A8: case PF_BYTE_LA:
		case PF_R8G8B8: case PF_B8G8R8:
		case PF_R8G8B8A8: case PF_B8G8R8A8:
		case PF_A8B8G8R8: case PF_A8R8G8B8:
		case PF_X8B8G8R8: case PF_X8R8G8B8:
			if(src.format == scaled.format) 
			{
				// No intermediate buffer needed
				temp = scaled;
			}
			else
			{
				// Allocate temp buffer of destination size in source format 
				temp = PixelBox(scaled.getWidth(), scaled.getHeight(), scaled.getDepth(), src.format);
				buf = CreateMemoryStream(temp.getConsecutiveSize(), false);
				temp.data = buf->GetPtr();
			}
			// super-optimized: byte-oriented math, no conversion
			switch (PixelUtil::getNumElemBytes(src.format)) 
			{
			case 1: LinearResampler_Byte<1>::scale(src, temp); break;
			case 2: LinearResampler_Byte<2>::scale(src, temp); break;
			case 3: LinearResampler_Byte<3>::scale(src, temp); break;
			case 4: LinearResampler_Byte<4>::scale(src, temp); break;
			default:
				// never reached
				ASSERT(false);
			}
			if(temp.data != scaled.data)
			{
				// Blit temp buffer
				PixelUtil::bulkPixelConversion(temp, scaled);
			}
			break;
		case PF_FLOAT32_RGB:
		case PF_FLOAT32_RGBA:
			if (scaled.format == PF_FLOAT32_RGB || scaled.format == PF_FLOAT32_RGBA)
			{
				// float32 to float32, avoid unpack/repack overhead
				LinearResampler_Float32::scale(src, scaled);
				break;
			}
			// else, fall through
		default:
			// non-optimized: floating-point math, performs conversion but always works
			LinearResampler::scale(src, scaled);
		}
		break;
	}
}

//-----------------------------------------------------------------------------    
uint32 CImageCodec::calculateSize(uint32 mipmaps, uint32 faces, uint32 width, uint32 height, uint32 depth, 
							PixelFormat format)
{
	uint32 size = 0;
	for( uint32 mip=0; mip<=mipmaps; ++mip )
	{
		size += PixelUtil::getMemorySize( width, height, depth, format )*faces; 
		if(width!=1) width /= 2;
		if(height!=1) height /= 2;
		if(depth!=1) depth /= 2;
	}
	return size;
}

void CImageCodec::applyGamma( unsigned char *buffer, float gamma, uint32 size, uint8 bpp )
{
	if( gamma == 1.0f )
		return;

	//NB only 24/32-bit supported
	if( bpp != 24 && bpp != 32 ) return;

	uint32 stride = bpp >> 3;

	for( uint32 i = 0, j = size / stride; i < j; i++, buffer += stride )
	{
		float r, g, b;

		r = (float)buffer[0];
		g = (float)buffer[1];
		b = (float)buffer[2];

		r = r * gamma;
		g = g * gamma;
		b = b * gamma;

		float scale = 1.0f, tmp;

		if( r > 255.0f && (tmp=(255.0f/r)) < scale )
			scale = tmp;
		if( g > 255.0f && (tmp=(255.0f/g)) < scale )
			scale = tmp;
		if( b > 255.0f && (tmp=(255.0f/b)) < scale )
			scale = tmp;

		r *= scale; g *= scale; b *= scale;

		buffer[0] = (uint8)r;
		buffer[1] = (uint8)g;
		buffer[2] = (uint8)b;
	}
}
//---------------------------------------------------------------------
bool CImageCodec::decode( const char* pszName, void* pMemory, uint32 nNumBytes, IN OUT ImageData& imgData ) const
{
	// Set error handler
	FreeImage_SetOutputMessage(FreeImageLoadErrorHandler);

	FIMEMORY* fiMem = 
		FreeImage_OpenMemory((BYTE*)pMemory, static_cast<uint32>(nNumBytes));

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(fiMem, static_cast<uint32>(nNumBytes));
	if(fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(pszName);
		if (fif == FIF_UNKNOWN)
		{
			ASSERT(false);
			FreeImage_CloseMemory(fiMem);
			return false;
		}
	}

	FIBITMAP* fiBitmap = FreeImage_LoadFromMemory(fif, fiMem);
	if (!fiBitmap)
	{
		//LogError("Error decoding image, FreeImageCodec::decode");
		FreeImage_Unload(fiBitmap);
		FreeImage_CloseMemory(fiMem);
		return false;
	}

	RefPtr<MemoryStream> output;
	imgData.m_nDepth = 1; // only 2D formats handled by this codec
	imgData.m_nWidth = FreeImage_GetWidth(fiBitmap);
	imgData.m_nHeight = FreeImage_GetHeight(fiBitmap);
	if (imgData.m_nWidth == 0 || imgData.m_nHeight == 0)
	{
		ASSERT(false);
		FreeImage_Unload(fiBitmap);
		FreeImage_CloseMemory(fiMem);
		return false;
	}

	imgData.m_nNumMipmaps = 0; // no mipmaps in non-DDS 
	imgData.m_nFlags = 0;

	// Must derive format first, this may perform conversions

	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(fiBitmap);
	FREE_IMAGE_COLOR_TYPE colourType = FreeImage_GetColorType(fiBitmap);
	unsigned bpp = FreeImage_GetBPP(fiBitmap);

	switch(imageType)
	{
	case FIT_UNKNOWN:
	case FIT_COMPLEX:
	case FIT_UINT32:
	case FIT_INT32:
	case FIT_DOUBLE:
	default:
		LogError("Unknown or unsupported image format, FreeImageCodec::decode");

		break;
	case FIT_BITMAP:
		// Standard image type
		// Perform any colour conversions for greyscale
		if (colourType == FIC_MINISWHITE || colourType == FIC_MINISBLACK)
		{
			FIBITMAP* newBitmap = FreeImage_ConvertToGreyscale(fiBitmap);
			// free old bitmap and replace
			FreeImage_Unload(fiBitmap);
			fiBitmap = newBitmap;
			// get new formats
			bpp = FreeImage_GetBPP(fiBitmap);
			colourType = FreeImage_GetColorType(fiBitmap);
		}
		// Perform any colour conversions for RGB
		else if (bpp < 8 || colourType == FIC_PALETTE || colourType == FIC_CMYK)
		{
			FIBITMAP* newBitmap =  NULL;	
			if (FreeImage_IsTransparent(fiBitmap))
			{
				// convert to 32 bit to preserve the transparency 
				// (the alpha byte will be 0 if pixel is transparent)
				newBitmap = FreeImage_ConvertTo32Bits(fiBitmap);
			}
			else
			{
				// no transparency - only 3 bytes are needed
				newBitmap = FreeImage_ConvertTo24Bits(fiBitmap);
			}

			// free old bitmap and replace
			FreeImage_Unload(fiBitmap);
			fiBitmap = newBitmap;
			// get new formats
			bpp = FreeImage_GetBPP(fiBitmap);
			colourType = FreeImage_GetColorType(fiBitmap);
		}

		// by this stage, 8-bit is greyscale, 16/24/32 bit are RGB[A]
		switch(bpp)
		{
		case 8:
			imgData.m_eFormat = PF_L8;
			break;
		case 16:
			// Determine 555 or 565 from green mask
			// cannot be 16-bit greyscale since that's FIT_UINT16
			if(FreeImage_GetGreenMask(fiBitmap) == FI16_565_GREEN_MASK)
			{
				imgData.m_eFormat = PF_R5G6B5;
			}
			else
			{
				// FreeImage doesn't support 4444 format so must be 1555
				imgData.m_eFormat = PF_A1R5G5B5;
			}
			break;
		case 24:
			// FreeImage differs per platform
			//     PF_BYTE_BGR[A] for little endian (== PF_ARGB native)
			//     PF_BYTE_RGB[A] for big endian (== PF_RGBA native)
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
			imgData.m_eFormat = PF_BYTE_RGB;
#else
			imgData.m_eFormat = PF_BYTE_BGR;
#endif
			break;
		case 32:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
			imgData.m_eFormat = PF_BYTE_RGBA;
#else
			imgData.m_eFormat = PF_BYTE_BGRA;
#endif
			break;


		};
		break;
	case FIT_UINT16:
	case FIT_INT16:
		// 16-bit greyscale
		imgData.m_eFormat = PF_L16;
		break;
	case FIT_FLOAT:
		// Single-component floating point data
		imgData.m_eFormat = PF_FLOAT32_R;
		break;
	case FIT_RGB16:
		imgData.m_eFormat = PF_SHORT_RGB;
		break;
	case FIT_RGBA16:
		imgData.m_eFormat = PF_SHORT_RGBA;
		break;
	case FIT_RGBF:
		imgData.m_eFormat = PF_FLOAT32_RGB;
		break;
	case FIT_RGBAF:
		imgData.m_eFormat = PF_FLOAT32_RGBA;
		break;


	};

	unsigned char* srcData = FreeImage_GetBits(fiBitmap);
	unsigned srcPitch = FreeImage_GetPitch(fiBitmap);

	// Final data - invert image and trim pitch at the same time
	uint32 dstPitch = imgData.m_nWidth * PixelUtil::getNumElemBytes(imgData.m_eFormat);
	imgData.m_nSize = dstPitch * imgData.m_nHeight;
	// Bind output buffer
	output = CreateMemoryStream(imgData.m_nSize, false);

	uint8* pDst = output->GetPtr();
	for (uint32 y = 0; y < imgData.m_nHeight; ++y)
	{
		uint8* pSrc = srcData + (imgData.m_nHeight - y - 1) * srcPitch;
		memcpy(pDst, pSrc, dstPitch);
		pDst += dstPitch;
	}


	FreeImage_Unload(fiBitmap);
	FreeImage_CloseMemory(fiMem);

	imgData.m_pMemory = output;

	/*if (imgData.format == PF_A8R8G8B8)
	{
		PixelBox src(imgData.width, imgData.height, imgData.depth, PF_A8R8G8B8, imgData.memory->GetPtr());

		PixelFormat destFormat = PF_A4R4G4B4;
		RefPtr<MemoryStream> dstMem = CreateMemoryStream(PixelUtil::getMemorySize(imgData.width, imgData.height, imgData.depth, destFormat), false);
		PixelBox dst(imgData.width, imgData.height, imgData.depth, destFormat, dstMem->GetPtr());
		PixelUtil::bulkPixelConversion(src, dst);
		imgData.format = destFormat;
		imgData.memory = dstMem;
	}*/
	return true;
}

}