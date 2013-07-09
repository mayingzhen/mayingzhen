#include "ResourceMangager.h"
#include "Engine/DataThread/DataThread.h"

namespace ma
{
	std::map<std::string,Resource*>			ResourceManager::m_resMap;
	std::map<std::string,ResourceCreator>	ResourceManager::m_resCreateFunMap;
	DataThread*								ResourceManager::m_pDataThread = NULL;


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
		std::string fullPath = FileSystem::getFullPath(pszRelPath);
		pResource->SetResPath(fullPath.c_str());

		m_resMap[pszRelPath] = pResource;

		if (m_pDataThread == NULL)
		{
			m_pDataThread = new DataThread(0,0);
		}

		m_pDataThread->PushBackDataObj(pResource);
	

		return pResource;
	}

	void ResourceManager::Process()
	{
		if (m_pDataThread == NULL)
		{
			m_pDataThread = new DataThread(0,0);
		}

		m_pDataThread->Process();

// 		if ( m_pDataThread->Process() )
// 		{
// 			while (true)
// 			{
// 				Resource* pRes = (Resource*)m_pDataThread->PopUpDataObj();
// 				ASSERT(pRes);
// 				if (pRes == NULL)
// 					continue;
// 
// 				pRes->OnLoaded();
// 			}
// 		}

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

