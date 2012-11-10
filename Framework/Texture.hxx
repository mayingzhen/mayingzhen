#include "Framework/Texture.h"

namespace ma
{
	Texture::Texture(const char* pTexturePath):
	Resource(pTexturePath)
	{
	}

	void Texture::Load()
	{
		if (m_pRendTexure == NULL)
		{
			m_pRendTexure = GetRender()->CreateRendTexture();
		}
		m_pRendTexure->Load( m_sResPath.c_str() );
	}
}