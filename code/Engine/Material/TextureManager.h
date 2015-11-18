#pragma once

namespace ma
{
	class TextureManager
	{
	public:
		TextureManager();

		~TextureManager();

		RefPtr<Texture> CreateTexture(const char* pImagePath,Wrap eWrap, Filter eFilter);

	private:
		typedef std::map<std::string, RefPtr<Texture> > ResourceMap;

		ResourceMap			m_resMap;
	};

	extern TextureManager* g_pTextureManager;
}

