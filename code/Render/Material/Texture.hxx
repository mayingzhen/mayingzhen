#include "Texture.h"



namespace ma
{
	IMPL_OBJECT(Texture,Resource);

	Texture::Texture(const char* pszPath):Resource(pszPath)
	{
		m_nWidth = 0;
		m_nHeight = 0;
		m_nMipLevels = 1;
		m_eUsage = USAGE_NO;
		m_eFormat = FMT_R8G8B8;
		m_eType = TEXTYPE_2D;
	}

	Texture::Texture(int nWidth,int nHeight,FORMAT eFormat,bool bDepthStencil)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nMipLevels = 0;
		m_eUsage = bDepthStencil ? USAGE_DEPTHSTENCIL : USAGE_RENDERTARGET;
		m_eFormat = eFormat;
		m_eType = TEXTYPE_2D;
	}

	void Texture::LoadSync()
	{
		LoadFileToMemeory();

		CreateFromMemeory();

		GetRenderSystem()->FlushAndWait();
	}

	bool Texture::CreateFromMemeory()
	{
		if (m_eResState == ResLoaded)
			return true;

		GetRenderSystem()->TexStreamComplete(this,m_pDataStream.get());
		
		m_eResState = ResLoaded;

		return true;
	}

}
