#include "GLESTexture.h"
#include "GLESMapping.h"


namespace ma
{
	GLESTexture::GLESTexture()
		:Texture()
	{
		m_pTex = 0;
		m_PixelFormat = 0;
	}

	GLESTexture::GLESTexture(int nWidth,int nHeight,UINT32 nMipMap, PixelFormat eFormat,bool bTypeLess,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType)
		:Texture(nWidth,nHeight,nMipMap,eFormat,bTypeLess,bSRGB,eUsage,eType)
	{
		m_pTex = 0;
		m_PixelFormat = 0;
		m_DataType = 0;
	}

	GLESTexture::~GLESTexture()
	{

	}

	PixelFormat GLESTexture::GetNativeFormat(PixelFormat format)
	{
		bool bSupportDXT = GetDeviceCapabilities()->GetTextureDXTSupported();
		bool bSupportPVRTC = GetDeviceCapabilities()->GetTexturePVRTCSupported();
		bool bSupportETC1 = GetDeviceCapabilities()->GetTextureETC1Supported();
		bool bSupportFloat = GetDeviceCapabilities()->GetFloatTexturesSupported();

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

	bool GLESTexture::RT_CreateCubeTexture()
	{
		ASSERT(m_pTex == 0);

		// Convert to nearest power-of-two size if required
		m_nWidth = Math::NextPowerOfTwo(m_nWidth);
		m_nHeight = Math::NextPowerOfTwo(m_nHeight);

		// Adjust format if required
		m_eFormat = this->GetNativeFormat(m_eFormat);

		m_PixelFormat = GLESMapping::GetGLESFormat(m_eFormat);
		m_DataType = GLESMapping::GetGLESDataType(m_eFormat);

		// Check requested number of mipmaps
		//size_t maxMips = getMaxMipmaps(m_nWidth, m_nHeight);

		// Generate texture name
		GL_ASSERT( glGenTextures(1, &m_pTex) );

		// Set texture type
		GL_ASSERT( glBindTexture(GL_TEXTURE_2D, m_pTex) );

		if (PixelUtil::isCompressed(m_eFormat))
		{
			UINT size = PixelUtil::getMemorySize(m_nWidth, m_nHeight, 1, m_eFormat);
			for(int face = 0; face < 6; face++) 
			{
				GL_ASSERT(glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0,m_PixelFormat,m_nWidth,m_nHeight,0,size,NULL) );
			}
		}
		else
		{
			for(int face = 0; face < 6; face++) 
			{
				GL_ASSERT(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, m_PixelFormat, m_nWidth, m_nHeight, 0, m_PixelFormat, m_DataType, NULL) );
			}
		}

#if GL_APPLE_texture_max_level
		GL_ASSERT( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL_APPLE, m_nMipLevels ) );
#endif

		return true;
	}

	void getBestDepthStencil(GLuint *depthFormat, GLuint *stencilFormat)
	{
#if GL_OES_packed_depth_stencil
		if(g_bGL_OES_packed_depth_stencil)
			*depthFormat = GL_DEPTH24_STENCIL8_OES;
		else
		{
			*depthFormat = GL_DEPTH_COMPONENT16;
		}
#else
		*depthFormat = GL_DEPTH_COMPONENT16;
#endif
		*stencilFormat = GL_STENCIL_INDEX8;
	}

	bool GLESTexture::RT_CreateTexture()
	{
		ASSERT(m_pTex == 0);

		// Convert to nearest power-of-two size if required
		m_nWidth = Math::NextPowerOfTwo(m_nWidth);
		m_nHeight = Math::NextPowerOfTwo(m_nHeight);

		// Adjust format if required
		m_eFormat = this->GetNativeFormat(m_eFormat);

		m_PixelFormat = GLESMapping::GetGLESFormat(m_eFormat);
		m_DataType = GLESMapping::GetGLESDataType(m_eFormat);

		// Check requested number of mipmaps
		size_t maxMips = getMaxMipmaps(m_nWidth, m_nHeight);

		// Generate texture name
		GL_ASSERT( glGenTextures(1, &m_pTex) );

		// Set texture type
		GL_ASSERT( glBindTexture(GL_TEXTURE_2D, m_pTex) );
		
		if (PixelUtil::isCompressed(m_eFormat))
		{
			UINT size = PixelUtil::getMemorySize(m_nWidth, m_nHeight, 1, m_eFormat);
			GL_ASSERT( glCompressedTexImage2D(GL_TEXTURE_2D,0,m_PixelFormat,m_nWidth,m_nHeight,0,size,NULL) );
		}
		else
		{
			GL_ASSERT( glTexImage2D(GL_TEXTURE_2D, 0, m_PixelFormat, m_nWidth, m_nHeight, 0, m_PixelFormat, m_DataType, NULL) );
		}

#if GL_APPLE_texture_max_level
		GL_ASSERT( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL_APPLE, m_nMipLevels ) );
#endif

		if (m_eUsage == USAGE_DEPTHSTENCIL)
		{
			GLuint depthFormat, stencilFormat;
			getBestDepthStencil(&depthFormat,&stencilFormat);

			GL_ASSERT( glGenRenderbuffers(1, &m_pTex) );

			GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, m_pTex) );

			GL_ASSERT( glRenderbufferStorage(GL_RENDERBUFFER, depthFormat, m_nWidth, m_nHeight) );
		}
		
		return true;
	}
 
	bool GLESTexture::SetLevelData(int level, int face, const PixelBox& src)
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
}

