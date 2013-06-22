
namespace ma
{
	bool TextureData::Load(const char* pszPath)
	{
		if (pszPath == NULL)
			pszPath = m_sResPath.c_str();

		m_pTexture = GetRenderDevice()->CreateRendTexture();
		m_pTexture->Load(pszPath);
		return true;
	}

	bool TextureData::Save(const char* pszPath)
	{
		return false;
	}	
}