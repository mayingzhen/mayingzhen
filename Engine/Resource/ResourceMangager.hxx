#include "Framework/ResourceMangager.h"

namespace ma
{

	std::string GetExtention(const std::string& FileName) 
	{
		int i;

		i = static_cast<int>(FileName.rfind("."));
		if(i == std::string::npos) return "";

		return FileName.substr(i + 1, FileName.size() - i - 1);
	}

	Resource* ResourceManager::DeclareResource(const char* pszRelPath)
	{
		if (pszRelPath == NULL)
			return;

		ResMap::iterator itRes = m_resMap.find(pszRelPath);
		if (itRes != m_resMap.end())
			return itRes->second;

		std::string fileExt = GetExtention(pszRelPath);
		ResCreateFunMap::iterator itFun = m_resCreateFunMap.find(fileExt);
		if (itFun == m_resCreateFunMap.end())
			return;
	
		Resource* pResource = itFun->second();
		pResource->SetResPath(pszRelPath);

		m_resMap[pszRelPath] = pResource;
	}

	void ResourceManager::RegisterResourceFactory(const char* fileExt,ResourceCreator pResCreator)
	{
		if (fileExt == NULL)
			return;

		ResCreateFunMap::iterator it = m_resCreateFunMap.find(fileExt);
		if (it == m_resCreateFunMap.end())
		{
			m_resCreateFunMap[fileExt] = pResCreator;
		}
	}

	void ResourceManager::UnregisterResourceFactory(const char* fileExt,ResourceCreator pResCreator)
	{
		if (fileExt == NULL)
			return;

		ResCreateFunMap::iterator it = m_resCreateFunMap.find(fileExt);
		if (it != m_resCreateFunMap.end() && it->second == pResCreator)
		{
			m_resCreateFunMap.erase(it);
		}
	}
}

