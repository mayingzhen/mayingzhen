#include "ResourceMangager.h"

namespace ma
{
	std::map<std::string,Resource*>			ResourceManager::m_resMap;
	std::map<std::string,ResourceCreator>	ResourceManager::m_resCreateFunMap;
	std::string								ResourceManager::m_sResRootPath;


	std::string GetExtention(const std::string& FileName) 
	{
		int i;

		i = static_cast<int>(FileName.rfind("."));
		if(i == std::string::npos) return "";

		return FileName.substr(i + 1, FileName.size() - i - 1);
	}

	void ResourceManager::SetResRootPath(const char* path)
	{
		m_sResRootPath = path;
	}

	const char* ResourceManager::GetResRootPath()
	{
		return m_sResRootPath.c_str();
	}

	Resource* ResourceManager::DeclareResource(const char* pszRelPath)
	{
		if (pszRelPath == NULL)
			return NULL;

		ResourceMap::iterator itRes = m_resMap.find(pszRelPath);
		if (itRes != m_resMap.end())
			return itRes->second;

		std::string fileExt = GetExtention(pszRelPath);
		ResCreateFunMap::iterator itFun = m_resCreateFunMap.find(fileExt);
		if (itFun == m_resCreateFunMap.end())
			return NULL;
	
		Resource* pResource = itFun->second();
		std::string fullPath = m_sResRootPath + pszRelPath;
		pResource->SetResPath(fullPath.c_str());

		m_resMap[pszRelPath] = pResource;

		return pResource;
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

