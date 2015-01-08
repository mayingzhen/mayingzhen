#include "GLESTexture.h"
#include "GLESMapping.h"


namespace ma
{
	GLESTexture::GLESTexture(const char* pszPath)
		:Texture(pszPath)
	{
		m_pTex = 0;
		m_PixelFormat = 0;
	}

	GLESTexture::GLESTexture(int nWidth,int nHeight,PixelFormat eFormat,USAGE eUsage)
		:Texture(nWidth,nHeight,eFormat,eUsage)
	{
		m_pTex = 0;
		m_PixelFormat = 0;
		m_DataType = 0;
	}

	GLESTexture::~GLESTexture()
	{

	}

	PixelFormat GLESTexture::getNativeFormat(PixelFormat format)
	{
		bool bSupportDXT = false;//GetDeviceCapabilities()->GetTextureDXTSupported();
		bool bSupportPVRTC = false;//GetDeviceCapabilities()->GetTexturePVRTCSupported();
		bool bSupportETC1 = false;//GetDeviceCapabilities()->GetTextureETC1Supported();
		bool bSupportFloat = false;//GetDeviceCapabilities()->getFloatTexturesSupported();

		// Check compressed texture support
		// if a compressed format not supported, revert to PF_A8R8G8B8
		if (PixelUtil::isCompressed(format) && !bSupportDXT && !bSupportPVRTC && !bSupportETC1)
		{
			return PF_A8R8G8B8;
		}

		// if floating point textures not supported, revert to PF_A8R8G8B8
		if (PixelUtil::isFloatingPoint(format) && !bSupportFloat)
		{
			return PF_A8R8G8B8;
		}

		// Supported
		return format;
	}

	size_t getMaxMipmaps(size_t width, size_t height)
    {
		size_t count = 0;
        if((width > 0) && (height > 0))
        {
            do {
                if(width>1)		width = width/2;
                if(height>1)	height = height/2;
                
                count ++;
            } while(!(width == 1 && height == 1));
        }		
		return count;
    }

	bool GLESTexture::RT_Create()
	{
		ASSERT(m_pTex == 0);

		// Convert to nearest power-of-two size if required
		m_nWidth = Math::NextPowerOfTwo(m_nWidth);
		m_nHeight = Math::NextPowerOfTwo(m_nHeight);

		// Adjust format if required
		m_eFormat = this->getNativeFormat(m_eFormat);

		m_PixelFormat = GLESMapping::GetGLESFormat(m_eFormat);
		m_DataType = GLESMapping::GetGLESDataType(m_eFormat);

		// Check requested number of mipmaps
		size_t maxMips = getMaxMipmaps(m_nWidth, m_nHeight);

		// Generate texture name
		GL_ASSERT( glGenTextures(1, &m_pTex) );

		// Set texture type
		GL_ASSERT( glBindTexture(GL_TEXTURE_2D, m_pTex) );

		GL_ASSERT( glTexImage2D(GL_TEXTURE_2D, 0, m_PixelFormat, m_nWidth, m_nHeight, 0, m_PixelFormat, m_DataType, NULL) );

#if GL_APPLE_texture_max_level
		GL_ASSERT( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL_APPLE, mNumMipmaps ) );
#endif

		// Set some misc default parameters, these can of course be changed later
		GL_ASSERT( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
		
		GL_ASSERT( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
		
		GL_ASSERT( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
		
		GL_ASSERT( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
	
	
		return true;
	}

	bool GLESTexture::SetLevelData(int level, const PixelBox& src)
	{
		GL_ASSERT( glBindTexture(GL_TEXTURE_2D, m_pTex) );

		int width = m_nWidth >> level;
		int height = m_nHeight >> level;
		
		PixelBox conver = src;
		PixelFormat closestFomat = GLESMapping::GetClosestFormat(m_PixelFormat,m_DataType);
		if (closestFomat != src.format)
		{	
			conver.format = closestFomat;
			PixelUtil::bulkPixelConversion(src,conver);
		}

		if (PixelUtil::isCompressed(m_eFormat))
		{
			UINT size = PixelUtil::getMemorySize(width, height, 1, m_eFormat);

			GL_ASSERT( glCompressedTexImage2D(GL_TEXTURE_2D,level,m_PixelFormat,width,height,0,size,conver.data) );
		}
		else
		{
			GL_ASSERT( glTexImage2D(GL_TEXTURE_2D,level,m_PixelFormat,width,height,0,m_PixelFormat,m_DataType,conver.data) );
		}

		return true;
	}

	bool GLESTexture::GenerateMipmaps()
	{
		GL_ASSERT( glBindTexture(GL_TEXTURE_2D, m_pTex) );
		GL_ASSERT( glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST) );
		GL_ASSERT( glGenerateMipmap(GL_TEXTURE_2D) );

		return true;
	}

// 	void GLESTexture::ConvertImageData(int pixelFormat,int nPixelCount,uint8* pPixel)
// 	{
// 		int nPixelSize = 1;
// 		if(pixelFormat == IL_BGR)
// 		{
// 			nPixelSize = 3;
// 		}
// 		else if(pixelFormat == IL_BGRA)
// 		{
// 			nPixelSize = 4;
// 		}
// 		else 
// 		{
// 			return;
// 		}
// 
// 		for(int i = 0; i < nPixelCount; ++i)
// 		{
// 			const int nIndex = i * nPixelSize;
// 			uint8 a = pPixel[nIndex];
// 			pPixel[nIndex] = pPixel[nIndex+2];
// 			pPixel[nIndex+2] = a;
// 		}
// 	}

}

