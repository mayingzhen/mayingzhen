#include "TextureManager.h"

namespace ma
{
	TextureManager::TextureManager()
	{

	}

	TextureManager::~TextureManager()
	{

	}

	RefPtr<Texture> TextureManager::CreateTexture(const char* pImagePath,bool bMipMap,bool bSRGB)
	{
		string strMipMap = StringConverter::toString(bMipMap);
		string strSRGB = StringConverter::toString(bSRGB);
		string strKey = string(pImagePath) + string("+") + strMipMap + string("+") +strSRGB + ".texture";
		StringUtil::toLowerCase(strKey);

		ResourceMap::iterator itRes = m_resMap.find(strKey);
		if (itRes != m_resMap.end())
			return itRes->second.get();

		RefPtr<Texture> pTextute = GetRenderDevice()->CreateTexture();
		pTextute->SetMipMap(bMipMap);
		pTextute->SetSRGB(bSRGB);
		pTextute->Load(pImagePath);
		m_resMap[strKey] = pTextute;
		return pTextute;	
	}	

	TextureManager* g_pTextureManager = NULL;

}
