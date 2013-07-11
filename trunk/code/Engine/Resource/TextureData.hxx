
namespace ma
{
	IMPL_OBJECT(TextureData,Resource)

	TextureData::TextureData(const char* pszPath)
	{
		m_pTexture = GetRenderDevice()->CreateRendTexture();
	}


	void TextureData::LoadImp(DataStream* pStream)
	{
		m_pTexture->Load(pStream);
	}

	bool TextureData::Save(const char* pszPath)
	{
		return false;
	}	
}