#include "GLESRender/GLESTexture.h"

#include "il/il.h"

namespace ma
{

	bool InitTextureSystem()
	{
		ilInit();
		ilEnable(IL_CONV_PAL);// 转换调色板数据
		ilClearColour(0.0f, 0.0f, 0.0f, 1.0f);

		// 定义图像原点为左上角,这样载入的图像在数据上是从上至下的.
		// 这与DX的纹理寻址方式一致,但与OpenGL的方式相反!
		ilEnable(IL_ORIGIN_SET);
		ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);

		// Delete the stupid first one image.
		ILuint image = 1;
		ilDeleteImages(1, &image);

#if defined(_WIN32)
		//ilSetEtcDecompress(IL_TRUE);
		ilSetInteger(IL_DXT_DONT_DECOMP, IL_TRUE);
		ilSetInteger(IL_KEEP_DXTC_DATA, IL_TRUE);
		//ilSetEtcDecompress(IL_TRUE);
#elif defined(__APPLE__)
#if defined(TARGET_OS_IPHONE)
		ilSetInteger(IL_DXT_DONT_DECOMP, IL_FALSE);
		ilSetInteger(IL_KEEP_DXTC_DATA, IL_FALSE);
#else
		ilSetInteger(IL_DXT_DONT_DECOMP, IL_TRUE);
		ilSetInteger(IL_KEEP_DXTC_DATA, IL_TRUE);
#endif//TARGET_OS_IPHONE
#elif defined(__ANDROID__)
		ilSetEtcDecompress(g_bHardwareETCSupported? IL_FALSE : IL_TRUE);
		ilSetInteger(IL_KEEP_DXTC_DATA, g_bHardwareDDSSupported? IL_TRUE : IL_FALSE);
		ilSetInteger(IL_DXT_DONT_DECOMP, g_bHardwareDDSSupported? IL_TRUE : IL_FALSE);
#endif

		return true;
	}

	bool ShundownTextureSystem()
	{
		// 留空概念备用
		ilShutDown();
		return true;
	}

	bool IsCompressedTextureFormat(ILenum format)
	{
		return((format == IL_PVRTCRGB2)
			|| (format == IL_PVRTCRGBA2)
			|| (format == IL_PVRTCRGB4)
			|| (format == IL_PVRTCRGBA4) 
			|| (format == IL_ETC_FORMAT)
			|| (format == IL_DXT1) 
			|| (format == IL_DXT3) 
			|| (format == IL_DXT5)
			|| (format == IL_ATC_FORMAT) 
			|| (format == IL_ATCA_FORMAT) 
			|| (format == IL_ATCI_FORMAT));
	}

	GLenum ChooseBestTextureFormat(GLenum srcImageFormat)
	{	
		switch(srcImageFormat)
		{	
		case IL_RGB:
		case IL_BGR:// Note: OpenGL ES NOT support GL_BGR
			{
				return GL_RGB;
			}
			break;

		case IL_RGBA:
		case IL_BGRA:
		//case IL_PVR_RGBA4444:
			{
				return GL_RGBA;
			}
			break;

		case IL_PVRTCRGB2:
			{
				return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
			}
			break;

		case IL_PVRTCRGBA2:
			{
				return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
			}
			break;

		case IL_PVRTCRGB4:
			{
				return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
			}
			break;

		case IL_PVRTCRGBA4:
			{
				return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
			}
			break;

		case IL_ETC_FORMAT:
			{
				return GL_ETC1_RGB8_OES;
			}
			break;

		case IL_ATC_FORMAT:
			{
				return GL_ATC_RGB_AMD;
			}
			break;

		case IL_ATCA_FORMAT:
			{
				return GL_ATC_RGBA_EXPLICIT_ALPHA_AMD;
			}
			break;

// 		case IL_ATCI_FORMAT:
// 			{
// 				return GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD;
// 			}
// 			break;
// 
// 		case IL_DXT1:
// 			{
// 				return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
// 			}
// 			break;
// 
// 		case IL_DXT3:
// 			{
// 				return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
// 			}
// 			break;
// 
// 		case IL_DXT5:
// 			{
// 				return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
// 			}
// 			break;
		}
		return 0;
	}	

	GLESTexture::GLESTexture()
	{
	}

	GLESTexture::~GLESTexture()
	{

	}

	bool GLESTexture::CreateRT(int nWidth,int nHeight,FORMAT Format)
	{
		if (nWidth == -1 && nHeight == -1)
		{
			GetRenderDevice()->GetRenderWndSize(nWidth,nHeight);
		}

		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_eFormat = Format;
		m_nMipLevels = 0;
		m_eUsage = USAGE_DYNAMIC;
		m_eType = TEXTYPE_RENDERTARGET;
		m_PixelFormat = GLESMapping::GetGLESFormat(Format);
			
		glGenTextures(1, &m_pTex);
		glBindTexture(GL_TEXTURE_2D, m_pTex);
		glTexImage2D(GL_TEXTURE_2D, 0, m_PixelFormat, nWidth, nHeight, 0, m_PixelFormat, GL_UNSIGNED_BYTE, NULL);
		
		return true;
	}

	bool GLESTexture::LoadFromData(FORMAT format,UINT width,UINT height,Uint8* data,UINT size, bool generateMipmaps)
	{
		m_PixelFormat = GLESMapping::GetGLESFormat(format);
		m_nWidth = width;
		m_nHeight = height;
		ConvertImageData(m_PixelFormat,m_nWidth * m_nHeight,data);
		
		glGenTextures(1, &m_pTex);
		glBindTexture(GL_TEXTURE_2D, m_pTex);

		glTexImage2D(GL_TEXTURE_2D, 0, m_PixelFormat, m_nWidth, m_nHeight, 0, m_PixelFormat, GL_UNSIGNED_BYTE, data);	

		if (generateMipmaps)
		{
			GenerateMipmaps();
		}

		return true;
	}

	bool GLESTexture::Load(const char* pszPath,bool generateMipmaps)
	{
		// 创建DevImage句柄
		ILuint curImage = 0; 
		ilGenImages(1, &curImage);
		ilBindImage(curImage);

		if(!ilLoadImage(pszPath))
		{ 
			ASSERT(false);
			ilDeleteImages(1, &curImage);
			return false;
		} 
	

		m_nWidth = ilGetInteger(IL_IMAGE_WIDTH);
		m_nHeight = ilGetInteger(IL_IMAGE_HEIGHT);
		m_nMipLevels = ilGetInteger(IL_NUM_MIPMAPS);
		ILint imageFormat = ilGetInteger(IL_IMAGE_FORMAT);
		m_PixelFormat = ChooseBestTextureFormat(imageFormat);
		bool bCompressFormat = IsCompressedTextureFormat(imageFormat);

		glGenTextures(1, &m_pTex);
		glBindTexture(GL_TEXTURE_2D, m_pTex);

		int i = 0;
		while(ilActiveMipmap(i))
		{	
			Uint8* pSrcData= (Uint8*)ilGetData();
			int unLevelWidth = ilGetInteger(IL_IMAGE_WIDTH);
			int unLevelHeight = ilGetInteger(IL_IMAGE_HEIGHT);

			ConvertImageData(m_PixelFormat,unLevelWidth * unLevelHeight,pSrcData);

			if(bCompressFormat)
			{
				UINT unDataLength = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
				glCompressedTexImage2D(GL_TEXTURE_2D, i, m_PixelFormat, unLevelWidth, unLevelHeight, 0, unDataLength, pSrcData);
			}	
			else
			{
				glTexImage2D(GL_TEXTURE_2D, i, m_PixelFormat, unLevelWidth, unLevelHeight, 0, m_PixelFormat, GL_UNSIGNED_BYTE, pSrcData);	
			}

			ilBindImage(curImage);
			i++;
		}

		ilDeleteImages(1, &curImage);

		if (generateMipmaps)
		{
			GenerateMipmaps();
		}
		
		glFlush(); // 少了这句会乱,似乎异步建立的OpenGL的顶点缓冲尚未实际建立数据,就被使用了.

		return true;
	}

	void GLESTexture::GenerateMipmaps()
	{
		if (m_nMipLevels <= 1)
		{
			GL_ASSERT( glBindTexture(GL_TEXTURE_2D, m_pTex) );
			GL_ASSERT( glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST) );
			GL_ASSERT( glGenerateMipmap(GL_TEXTURE_2D) );
		}
	}

	void GLESTexture::ConvertImageData(GLenum pixelFormat,int nPixelCount,Uint8* pPixel)
	{
		int nPixelSize = 1;
		if(pixelFormat == GL_BGR)
		{
			nPixelSize = 3;
		}
		else if(pixelFormat == GL_BGRA)
		{
			nPixelSize = 4;
		}
		else if (pixelFormat == GL_ALPHA)
		{
			return;
			//nPixelSize = 1;
		}

		for(int i = 0; i < nPixelCount; ++i)
		{
			const int nIndex = i * nPixelSize;
			Uint8 a = pPixel[nIndex];
			pPixel[nIndex] = pPixel[nIndex+2];
			pPixel[nIndex+2] = a;
		}
	}
}

