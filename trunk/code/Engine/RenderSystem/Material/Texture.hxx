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
		m_eFormat = FMT_R8G8B8;
		m_eType = TEXTYPE_2D;
	}

	Texture::Texture(int nWidth,int nHeight,FORMAT format)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nMipLevels = 0;
		m_eUsage = USAGE_DYNAMIC;
		m_eFormat = format;
		m_eType = TEXTYPE_RENDERTARGET;
	}

	void Texture::LoadSync()
	{
		LoadFileToMemeory();

		CreateFromMemeory();

		GetRenderThread()->FlushAndWait();
	}

	bool Texture::CreateFromMemeory()
	{
		if (m_eResState == ResLoaded)
			return true;

		GetRenderThread()->RC_TexStreamComplete(this,m_pDataStream);

		return true;
	}
}
