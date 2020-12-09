#include "Texture.h"
#include "TextureManager.h"


namespace ma
{
	Texture::Texture()
	{
		m_nWidth = 0;
		m_nHeight = 0;
		m_nMipLevels = 1;
		m_eUsage = TEXTURE_USAGE::USAGE_STATIC;
		m_eFormat = PF_UNKNOWN;
		m_eType = TEXTYPE_2D;
		m_bMipMap = false;
		m_bSRGB = false;
	}

	Texture::Texture(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat eFormat,bool bSRGB,TEXTURE_USAGE eUsage,TEXTURE_TYPE eType) 
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nMipLevels = std::max<uint32_t>(nMipMap,1);
		m_eUsage = eUsage;
		m_eFormat = eFormat;
		m_bSRGB = bSRGB;
		m_eType = eType;
		m_bMipMap = false;
	}

	Texture::~Texture()
	{

	}

	bool Texture::InitRes() 
	{
		const char* pszName = this->GetResPath();
		void* pMemory = m_pDataStream->GetPtr();
		uint32_t nSizeBytes = m_pDataStream->GetSize();

		m_pImageData = new ImageData();
		if (!BuildImageData(pszName, pMemory, nSizeBytes, *m_pImageData))
		{
			return false;
		}
		
		SetImageData(m_pImageData.get());

		return true;
	}

	void Texture::SetImageData(ImageData* pImageData)
	{
		m_pImageData = pImageData;

		m_nWidth = m_pImageData->m_nWidth;
		m_nHeight = m_pImageData->m_nHeight;
		m_nMipLevels = std::max<uint32_t>(1, m_pImageData->m_nNumMipmaps);

		GetRenderSystem()->TexStreamComplete(this);
	}


	bool Texture::RT_StreamComplete()
	{
		ASSERT(m_pImageData);
		if (m_pImageData == NULL)
			return false;

		if (m_pImageData->getNumFaces() == 6)
		{
			m_eType = TEXTYPE_CUBE;
		}

		if ( !LoadFromImagData(*m_pImageData) )
		{
			return false;
		}

		m_pImageData = NULL;

		return true;
	}

	bool Texture::BuildImageData(const char* pszFile, void* pMemory, uint32_t nNumBytes, OUT ImageData& imageData)
	{
		if (StringUtil::endsWith(pszFile, ".pvr", true))
		{
			CPVRTCCodec pvr;
			if (!pvr.decode(pszFile, pMemory, nNumBytes, imageData))
			{
				LogError("Failed to decode pvr:%s", pszFile);
				return false;
			}

			if (PixelUtil::isCompressed(imageData.m_eFormat))
			{
				if(imageData.m_eFormat == PF_ETC1_RGB8)
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

			if (PixelUtil::isCompressed(imageData.m_eFormat) && !GetDeviceCapabilities()->GetTextureETC1Supported())
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

			if (PixelUtil::isCompressed(imageData.m_eFormat) && !GetDeviceCapabilities()->GetTextureDXTSupported())
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

	RefPtr<Texture> CreateTexture(const char* pImagePath,bool bMipMap/* = true*/,bool bSRGB/* = true*/)
	{
		return g_pTextureManager->CreateTexture(pImagePath,bMipMap,bSRGB);
	}

	RefPtr<Texture> CreateTexture(ImageData* imageData, bool bMipMap/* = true*/, bool bSRGB/* = true*/)
	{
		RefPtr<Texture> pTextute = GetRenderDevice()->CreateTexture();
		pTextute->SetMipMap(bMipMap);
		pTextute->SetSRGB(bSRGB);
		pTextute->SetImageData(imageData);
		return pTextute;
	}

	SamplerState::SamplerState()
	{
		m_eWrap = REPEAT;
		m_eWrapW = REPEAT;
		m_eFilter = TFO_TRILINEAR;
		m_bSRGB = true;
		m_fLodBias = 0;
	}

	void SamplerState::RegisterAttribute()
	{
		ENUM_ACCESSOR_ATTRIBUTE(SamplerState, "Wrap", GetWrapMode, SetWrapMode,Wrap,strDescWrap,REPEAT, AM_DEFAULT);
		ENUM_ACCESSOR_ATTRIBUTE(SamplerState, "WrapW", GetWrapModeW, SetWrapModeW,Wrap,strDescWrap,REPEAT, AM_DEFAULT);
		ENUM_ACCESSOR_ATTRIBUTE(SamplerState, "Filter", GetFilterMode, SetFilterMode, Filter, strDescFilter, TFO_TRILINEAR, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(SamplerState, "SRGB", GetSRGB, SetSRGB, bool, true, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(SamplerState, "LodBias", GetLodBias, SetLodBias, float, 0, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(SamplerState, "ImagePath", GetTexturePath, SetTexturePath, const char*, NULL, AM_DEFAULT);
	}

	void SamplerState::SetTexturePath(const char* pszPath)
	{
		m_pTexture = CreateTexture(pszPath,m_eFilter > TFO_BILINEAR, m_bSRGB);
	}

	const char*	SamplerState::GetTexturePath() const
	{
		return m_pTexture->GetResPath();
	}

	bool SamplerState::operator < ( const SamplerState& other) const
	{
		if (m_eWrap < other.m_eWrap)
			return true;
		else if(m_eWrap > other.m_eWrap)
			return false;

		if (m_eWrapW < other.m_eWrapW)
			return true;
		else if(m_eWrapW > other.m_eWrapW)
			return false;

		if (m_eFilter < other.m_eFilter)
			return true;
		else if(m_eFilter > other.m_eFilter)
			return false;

		if (m_bSRGB < other.m_bSRGB)
			return true;
		else if(m_bSRGB > other.m_bSRGB)
			return false;

		if (m_fLodBias < other.m_fLodBias)
			return true;
		else if (m_fLodBias > other.m_fLodBias)
			return false;

		return false;
	}

	RefPtr<SamplerState> CreateSamplerState(const char* pPath,Wrap eWrap, Filter eFilter,bool bSRGB)
	{
		SamplerState* pSampler = GetRenderDevice()->CreateSamplerState();
		pSampler->SetWrapMode(eWrap);
		pSampler->SetFilterMode(eFilter);
		pSampler->SetSRGB(bSRGB);
		pSampler->SetTexturePath(pPath);
        GetRenderSystem()->RC_AddRenderCommad( [pSampler] {pSampler->RT_StreamComplete();} );
		return pSampler;
	}

	RefPtr<SamplerState> CreateSamplerState(Texture* pTexutre,Wrap eWrap, Filter eFilter,bool bSRGB)
	{
		SamplerState* pSampler = GetRenderDevice()->CreateSamplerState();
		pSampler->SetWrapMode(eWrap);
		pSampler->SetFilterMode(eFilter);
		pSampler->SetSRGB(bSRGB);
		pSampler->SetTexture(pTexutre);
        GetRenderSystem()->RC_AddRenderCommad( [pSampler] {pSampler->RT_StreamComplete();} );
		return pSampler;
	}


}

