#include "Texture.h"



namespace ma
{
	IMPL_OBJECT(Texture,Resource);

	Texture::Texture(const char* pszPath):Resource(pszPath)
	{
		m_nWidth = 0;
		m_nHeight = 0;
		m_nMipLevels = 1;
		m_eUsage = USAGE_STATIC;
		m_eFormat = PF_UNKNOWN;
		m_eType = TEXTYPE_2D;
		m_bAutoMipMap = true;
	}

	Texture::Texture(int nWidth,int nHeight,PixelFormat eFormat,USAGE eUsage)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nMipLevels = 1;
		m_eUsage = eUsage;
		m_eFormat = eFormat;
		m_eType = TEXTYPE_2D;
		m_bAutoMipMap = false;
	}

	void Texture::LoadSync()
	{
		LoadFileToMemeory();

		CreateFromMemeory();

		GetRenderSystem()->FlushAndWait();
	}

	bool Texture::CreateFromMemeory()
	{
		ASSERT(m_eResState == ResLoaded);
		if (m_eResState != ResLoaded)
			return false;

		GetRenderSystem()->TexStreamComplete(this);	

		return true;
	}

	bool Texture::RT_StreamComplete()
	{
		ASSERT(m_pDataStream);
		if (m_pDataStream == NULL)
			return false;

		ImageData imageData;
		if (!BuildImageData(this->GetResPath(), m_pDataStream->GetPtr(), m_pDataStream->GetSize(), imageData))
		{
			return false;
		}

		if ( !LoadFromImagData(imageData) )
		{
			return false;
		}

		m_eResState = ResInited;

		m_pDataStream = NULL;

		return true;
	}


	bool Texture::BuildImageData(const char* pszFile, void* pMemory, uint32 nNumBytes, OUT ImageData& imageData)
	{
		if (StringUtil::endsWith(pszFile, ".pvr", true))
		{
			CPVRTCCodec pvr;
			if (!pvr.decode(pszFile, pMemory, nNumBytes, imageData))
			{
				LogError("Failed to decode pvr:%s", pszFile);
				return false;
			}

			if (PixelUtil::isCompressed(imageData.format))
			{
				if(imageData.format == PF_ETC1_RGB8)
				{
					if(!GetDeviceCapabilities()->GetTextureETC1Supported())
					{
						ETC1Decompress(imageData);
						LogWarning("Decompress etc texture to ARGB8 :%s", pszFile);
					}
				}
				else
				{
					if(!GetDeviceCapabilities()->GetTexturePVRTCSupported())
					{
						PVRTCDecompress(imageData);
						LogWarning("Decompress pvr texture to ARGB8 :%s", pszFile);
					}
				}
			}
		}
		else if (StringUtil::endsWith(pszFile, ".pkm", true))
		{
			CETC1Codec etc1;
			if (!etc1.decode(pszFile, pMemory, nNumBytes, imageData))
			{
				LogError("Failed to decode etc:%s", pszFile);
				return false;
			}

			if (PixelUtil::isCompressed(imageData.format) && !GetDeviceCapabilities()->GetTextureETC1Supported())
			{
				ETC1Decompress(imageData);
				LogWarning("Decompress etc texture to ARGB8 :%s", pszFile);
			}
		}
		else if (StringUtil::endsWith(pszFile, ".dds", true))
		{
			CDDSCodec dds;
			if (!dds.decode(pszFile, pMemory, nNumBytes, imageData))
			{
				LogError("Failed to decode dds:%s", pszFile);
				return false;
			}

			if (PixelUtil::isCompressed(imageData.format) && !GetDeviceCapabilities()->GetTextureDXTSupported())
			{
				DXTDecompress(imageData);
				LogWarning("Decompress dxt texture to ARGB8 :%s", pszFile);
			}
		}
		else
		{
			CImageCodec codec;
			if (!codec.decode(pszFile, pMemory, nNumBytes, imageData))
			{
				CDDSCodec dds;
				if (!dds.decode(pszFile, pMemory, nNumBytes, imageData))
				{
					LogError("Failed to decode dds:%s", pszFile);
					return false;
				}
			}
		}

		return true;
	}

	bool Texture::LoadFromImagData( const ImageData& imageData )
	{
		// Set desired texture size and properties from images[0]
		m_nWidth = imageData.width;
		m_nHeight = imageData.height;

		// Get source image format and adjust if required
		m_eFormat = imageData.format;

		// The custom mipmaps in the image have priority over everything
		//size_t imageMips = imageData.num_mipmaps;

		if(imageData.num_mipmaps > 0)
		{
			m_nMipLevels = imageData.num_mipmaps;
			// Disable flag for auto mip generation
			m_bAutoMipMap  = false;
		}

		// Create the texture
		if (!RT_CreateTexture())
		{
			LogError("Failed to createInternalResources:%d, %d, %s, %d", m_nWidth, m_nHeight, this->GetResPath(), m_eFormat);
			return false;
		}

		for(size_t mip = 0; mip <= imageData.num_mipmaps && mip < m_nMipLevels; ++mip)
		{
			PixelBox src = imageData.GetPixelBox(0, mip);

			src.format = m_eFormat;

			SetLevelData(mip,src);
		}

		if (m_bAutoMipMap)
		{
			GenerateMipmaps();
		}

		m_eResState = ResInited;

		return true;
	}

	RefPtr<Texture> CreateTexture(const char* pPath,bool bMipmap)
	{
		Texture* pTextute = DeclareResource<Texture>(pPath);
		pTextute->SetAutoMipMap(bMipmap);
		pTextute->Load();
		return pTextute;
	}
}

