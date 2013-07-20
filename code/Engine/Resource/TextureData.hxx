
namespace ma
{
	IMPL_OBJECT(TextureData,Resource)

	TextureData::TextureData(const char* pszPath)
	{
		m_pTexture = GetRenderDevice()->CreateRendTexture();
	}


	void TextureData::CreateFromMemeory()
	{
		if (m_eResState == ResLoaded)
			return;

		m_pTexture->Load(m_pDataStream);

		m_eResState = ResLoaded;
	}

	bool TextureData::Save(const char* pszPath)
	{
		return false;
	}	
}