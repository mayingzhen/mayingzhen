#include "ResourceMangager.h"
#include "Engine/DataThread/DataThread.h"

namespace ma
{
	std::map<std::string,Resource*>			ResourceManager::m_resMap;
	std::map<std::string,ResourceCreator>	ResourceManager::m_resCreateFunMap;

	Resource* ResourceManager::DeclareResource(const char* pszRelPath)
	{
		if (pszRelPath == NULL)
			return NULL;

		ResourceMap::iterator itRes = m_resMap.find(pszRelPath);
		if (itRes != m_resMap.end())
			return itRes->second;

		std::string fileExt = FileSystem::getExtension(pszRelPath);
		ResCreateFunMap::iterator itFun = m_resCreateFunMap.find(fileExt);
		if (itFun == m_resCreateFunMap.end())
			return NULL;
	
		Resource* pResource = itFun->second();
		pResource->SetResPath(pszRelPath);

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

