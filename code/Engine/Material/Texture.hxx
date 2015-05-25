#include "Texture.h"



namespace ma
{
	IMPL_OBJECT(Texture,Serializable);

	Texture::Texture()
	{
		m_nWidth = 0;
		m_nHeight = 0;
		m_nMipLevels = 1;
		m_eUsage = USAGE_STATIC;
		m_eFormat = PF_UNKNOWN;
		m_eType = TEXTYPE_2D;

		m_eWrap = REPEAT;
		m_eFilter = TFO_TRILINEAR;
	}

	Texture::Texture(int nWidth,int nHeight,PixelFormat eFormat,USAGE eUsage) 
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nMipLevels = 1;
		m_eUsage = eUsage;
		m_eFormat = eFormat;
		m_eType = TEXTYPE_2D;

		m_eWrap = CLAMP;
		m_eFilter = TFO_POINT;
	}

	Texture::~Texture()
	{

	}

	void Texture::RegisterAttribute()
	{
		ENUM_ACCESSOR_ATTRIBUTE(Texture, "Wrap", GetWrapMode, SetWrapMode, Wrap, strDescWrap, REPEAT, AM_DEFAULT);
		ENUM_ACCESSOR_ATTRIBUTE(Texture, "Filter", GetFilterMode, SetFilterMode, FilterOptions, strDescFilterOptions, TFO_TRILINEAR, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(Texture, "Texture", GetImagePath, SetImagePath, const char*, NULL, AM_DEFAULT);
	}

	const char* Texture::GetImagePath() const
	{
		return m_pImageRes ? m_pImageRes->GetResPath() : m_strImagePath.c_str();
	}

	void Texture::SetImagePath(const char* pTexPath)
	{
		m_strImagePath = pTexPath;
		m_pImageRes = CreateResource(pTexPath);
	}

	bool Texture::OnLoadOver()
	{
		if (ResInited == m_eResState)
			return true;

		if (m_pImageRes == NULL || !m_pImageRes->OnLoadOver())
			return false;
		
		GetRenderSystem()->TexStreamComplete(this);	

		return false;
	}

	bool Texture::RT_StreamComplete()
	{
		ASSERT(m_pImageRes);
		if (m_pImageRes == NULL)
			return false;

		const char* pszName = m_pImageRes->GetResPath();
		void* pMemory = m_pImageRes->GetDataStream()->GetPtr(); 
		uint32 nSizeBytes =	m_pImageRes->GetDataStream()->GetSize();

		ImageData imageData;
		if (!BuildImageData(pszName, pMemory, nSizeBytes, imageData))
		{
			return false;
		}

		if ( !LoadFromImagData(imageData) )
		{
			return false;
		}

		m_pImageRes = NULL;
		m_pDataStream = NULL;

		m_eResState = ResInited;

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

		bool bAutoMipMap = m_eFilter > TFO_BILINEAR;

		if(imageData.num_mipmaps > 0)
		{
			m_nMipLevels = imageData.num_mipmaps;
			// Disable flag for auto mip generation
			//bAutoMipMap  = false;
		}

		// Create the texture
		if (!RT_CreateTexture(bAutoMipMap))
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

		if (bAutoMipMap)
		{
			GenerateMipmaps();
		}

		m_eResState = ResInited;

		return true;
	}

	RefPtr<Texture> CreateSamplerState(const char* pImagePath,Wrap eWrap, FilterOptions eFilter)
	{
		string strTemp = string(pImagePath) + string("+") + strDescWrap[eWrap] + string("+") + strDescFilterOptions[eFilter] + ".sampler";
		RefPtr<Texture> pSamplerState = FindResource<Texture>( strTemp.c_str() );
		if (pSamplerState)
			return pSamplerState;

		Texture* pTextute = DeclareResource<Texture>(strTemp.c_str());
		pTextute->SetWrapMode(eWrap);
		pTextute->SetFilterMode(eFilter);
		pTextute->SetImagePath(pImagePath);
		pTextute->OnLoadOver();
		return pTextute;
	}

}

