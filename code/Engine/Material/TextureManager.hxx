#include "TextureManager.h"

namespace ma
{
	TextureManager::TextureManager()
	{

	}

	TextureManager::~TextureManager()
	{

	}

	RefPtr<Texture> TextureManager::CreateTexture(const char* pImagePath,Wrap eWrap, Filter eFilter)
	{
		string strKey = string(pImagePath) + string("+") + strDescWrap[eWrap] + string("+") + strDescFilter[eFilter] + ".texture";
		StringUtil::toLowerCase(strKey);

		ResourceMap::iterator itRes = m_resMap.find(strKey);
		if (itRes != m_resMap.end())
			return itRes->second.get();

		RefPtr<Texture> pTextute = GetRenderDevice()->CreateTexture();
		pTextute->Load(pImagePath,eWrap,eFilter);
		m_resMap[strKey] = pTextute;
		return pTextute;	
	}	

	TextureManager* g_pTextureManager = NULL;

}
