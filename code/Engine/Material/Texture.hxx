#include "Texture.h"



namespace ma
{
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

	const char* Texture::GetImagePath() const
	{
		return m_pImageRes ? m_pImageRes->GetResPath() : m_strImagePath.c_str();
	}

	void Texture::Load(const char* pszPath,Wrap eWrap,Filter eFilter)
	{
		m_eWrap = eWrap;
		m_eFilter = eFilter;
		m_strImagePath = pszPath;
		m_pImageRes = CreateResource(pszPath);
		IsReady();
	}

	bool Texture::IsReady()
	{
		if (ResInited == m_eResState)
			return true;

		if (m_pImageRes == NULL || !m_pImageRes->IsReady())
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

	RefPtr<Texture> Texture::Improt(TiXmlElement* pXmlTexture)
	{
		const char* pszIMagePath = pXmlTexture->Attribute("ImagePath");
		const char* pszWrap = pXmlTexture->Attribute("Wrap");
		const char* pszFilter = pXmlTexture->Attribute("Filter");

		Wrap eWrap = StringToEnum<Wrap>(pszWrap,strDescWrap);
		Filter eFilter = StringToEnum<Filter>(pszFilter,strDescFilter);

		RefPtr<Texture> pTexture = CreateTexture(pszIMagePath,eWrap,eFilter);
		return pTexture;
	}

	void Texture::Export(Texture* pTexutre,TiXmlElement* pXmlTexture)
	{
		pXmlTexture->SetAttribute("ImagePath",pTexutre->GetImagePath());
		pXmlTexture->SetAttribute("Wrap",strDescWrap[pTexutre->GetWrapMode()]);
		pXmlTexture->SetAttribute("Filter",strDescFilter[pTexutre->GetFilterMode()]);
	}

	RefPtr<Texture> CreateTexture(const char* pImagePath,Wrap eWrap, Filter eFilter)
	{
		string strKey = string(pImagePath) + string("+") + strDescWrap[eWrap] + string("+") + strDescFilter[eFilter] + ".texture";
		StringUtil::toLowerCase(strKey);
		RefPtr<Texture> pTextute = FindResource<Texture>( strKey.c_str() );
		if (pTextute)
			return pTextute;

		pTextute = GetRenderDevice()->CreateTexture();
		pTextute->Load(pImagePath,eWrap,eFilter);
		GetResourceSystem()->AddResource(strKey.c_str(),pTextute.get());
		return pTextute;
	}

}

