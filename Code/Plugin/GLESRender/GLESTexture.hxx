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

	struct IMAGE_INFO
	{
		GLuint Width;
		GLuint Height;
		GLuint PixelFormat;
	};


	GLESTexture::GLESTexture()
	{
		_mipmapped = false;
		m_nMemSize = 0;
		m_uvAddressMode = GL_REPEAT;
	}

	GLESTexture::~GLESTexture()
	{

	}

	bool GLESTexture::Create(int nWidth,int nHeight,int nMipLevel,FORMAT Format,USAGE Usage)
	{
		if (nWidth == -1 && nHeight == -1)
		{

			GetRenderDevice()->GetRenderWndSize(nWidth,nHeight);
		}

		ASSERT(nWidth && nHeight);



		return true;
	}

	void	GLESTexture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_pTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 		if(ilHasMipMap())
// 		{
// 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
// 		}
// 		else if(m_bAutoGenerateMipmap)
// 		{	
// 			// 没有Mipmap数据,需要自动生成.
// 			//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);// 自动产生Mipmap
// 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
// 		}
// 		else
// 		{
// 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 		}
// 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_uvAddressMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_uvAddressMode);
	}

	bool GLESTexture::Load(const char* pszPath,bool generateMipmaps)
	{
		// 创建DevImage句柄
		ILuint curImage = 0; 
		ilGenImages(1, &curImage);
		ilBindImage(curImage);

		//GLuint idNewTexture = 0;

		char szFullPathName[512] = { 0 };
		//sprintf(szFullPathName, "%s/%s", g_szAppFolderPath, pszName);
		if(!ilLoadImage(pszPath))
		{ 
			ASSERT(false);
			ilDeleteImages(1, &curImage);
			//SAFE_DELETE(pNewC3Tex);
			return false;
		} 
	
		IMAGE_INFO infoImage;
		infoImage.Width = ilGetInteger(IL_IMAGE_WIDTH);
		infoImage.Height = ilGetInteger(IL_IMAGE_HEIGHT);
		infoImage.PixelFormat = ilGetInteger(IL_IMAGE_FORMAT);
		m_PixelFormat = ChooseBestTextureFormat(infoImage.PixelFormat);
		if(ilGetInteger(IL_IMAGE_PVR_ADDBORDER)== IL_FALSE)
		{
			m_bAddBorderPvr = false;
		}
		else
		{
			m_bAddBorderPvr = true;
		}

		glGenTextures(1, &m_pTex);
		glBindTexture(GL_TEXTURE_2D, m_pTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if(ilHasMipMap())
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else if(m_bAutoGenerateMipmap)
		{	
			// 没有Mipmap数据,需要自动生成.
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);// 自动产生Mipmap
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//generateMipmaps();
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_uvAddressMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_uvAddressMode);
		//pNewC3Tex->uvAddressMode = texUVMode;
		//pNewC3Tex->nMemSize = sizeof(C3Texture);

		bool bCompressFormat = IsCompressedTextureFormat(infoImage.PixelFormat);

		int i = 0;
		Uint8* pSrcData = NULL;
		unsigned int unDataLength = 0;
		unsigned int unLevelWidth = 0;
		unsigned int unLevelHeight = 0;
		GLuint unLevelFormat = infoImage.PixelFormat;
		while(ilActiveMipmap(i))
		{	
			pSrcData= (Uint8*)ilGetData();
			unLevelWidth = ilGetInteger(IL_IMAGE_WIDTH);
			unLevelHeight = ilGetInteger(IL_IMAGE_HEIGHT);

			if(infoImage.PixelFormat == IL_BGR)
			{
				Uint8* pPixel = (Uint8*)pSrcData;
				const int nPixelCount = unLevelWidth * unLevelHeight;
				for(int i = 0; i < nPixelCount; ++i)
				{
					const int nIndex = i * 3;
					Uint8 a = pPixel[nIndex];
					pPixel[nIndex] = pPixel[nIndex+2];
					pPixel[nIndex+2] = a;
				}
				unLevelFormat = GL_RGB;
			}
			else if(infoImage.PixelFormat == IL_BGRA)
			{
				Uint8* pPixel = (Uint8*)pSrcData;
				const int nPixelCount = unLevelWidth * unLevelHeight;
				for(int i = 0; i < nPixelCount; ++i)
				{
					const int nIndex = i * 4;
					Uint8 a = pPixel[nIndex];
					pPixel[nIndex] = pPixel[nIndex+2];
					pPixel[nIndex+2] = a;
				}
				unLevelFormat = GL_RGBA;
			}

			if(bCompressFormat)
			{
				unDataLength = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
				m_nMemSize += unDataLength;
				glCompressedTexImage2D(GL_TEXTURE_2D, i, m_PixelFormat, unLevelWidth, unLevelHeight, 0, unDataLength, pSrcData);
			}
// 			else if(unLevelFormat == IL_PVR_RGBA4444)
// 			{
// 				pNewC3Tex->nMemSize += (unLevelWidth * unLevelHeight * 2);
// 				glTexImage2D(GL_TEXTURE_2D, i, PixelFormat, unLevelWidth, unLevelHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, pSrcData);	
// 			}
			else
			{
				m_nMemSize += (unLevelWidth * unLevelHeight * 5);// 夸大占用以促使pvr的使用.
				glTexImage2D(GL_TEXTURE_2D, i, m_PixelFormat, unLevelWidth, unLevelHeight, 0, unLevelFormat, GL_UNSIGNED_BYTE, pSrcData);	
			}

			ilBindImage(curImage);
			i++;
		}

		ilDeleteImages(1, &curImage);

		// 少了这句会乱,似乎异步建立的OpenGL的顶点缓冲尚未实际建立数据,就被使用了.
		glFlush();

		return true;
	}

	void GLESTexture::generateMipmaps()
	{
		if (!_mipmapped)
		{
			GL_ASSERT( glBindTexture(GL_TEXTURE_2D, m_pTex) );
			GL_ASSERT( glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST) );
			GL_ASSERT( glGenerateMipmap(GL_TEXTURE_2D) );

			_mipmapped = true;
		}
	}
}

