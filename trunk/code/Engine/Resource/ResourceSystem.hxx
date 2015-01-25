#include "ResourceSystem.h"

namespace ma
{
	static  ResourceSystem*	gpResourceSystem = NULL;

	ResourceSystem*	GetResourceSystem()
	{
		return gpResourceSystem;
	}


	void SetResourceSystem(ResourceSystem* pResSystem)
	{
		gpResourceSystem = pResSystem;
	}

	ResourceSystem::ResourceSystem()
	{
		m_pDataThread = NULL;
	}

	void ResourceSystem::Init(bool bDataThread)
	{
		SetDataThreadEnable(bDataThread);
	}

	void ResourceSystem::Shoutdown()
	{
		m_resMap.clear();
		m_resCreateFunMap.clear();

		if (m_pDataThread)
		{
			m_pDataThread->Stop();
			SAFE_DELETE(m_pDataThread);
		}
	}

	void ResourceSystem::SetDataThreadEnable(bool b)
	{
		if (b)
		{
			if (m_pDataThread == NULL)
			{
				m_pDataThread = new DataThread();
				m_pDataThread->Start();
			}
		}
		else
		{
			if (m_pDataThread)
			{
				m_pDataThread->Stop();
				SAFE_DELETE(m_pDataThread);
			}
		}
	}

	void ResourceSystem::Update()
	{
		if (m_pDataThread)
			m_pDataThread->Process();
	}

	DataThread*	ResourceSystem::GetDataThread()
	{	
		return m_pDataThread;
	}

	Resource* ResourceSystem::DeclareResource(const char* pszRelPath)
	{
		if (pszRelPath == NULL)
			return NULL;

		ResourceMap::iterator itRes = m_resMap.find(pszRelPath);
		if (itRes != m_resMap.end())
			return itRes->second.get();

		Resource* pResource = NULL;

		std::string fileExt = StringUtil::getFileExt(pszRelPath);
		ResCreateFunMap::iterator itFun = m_resCreateFunMap.find(fileExt);
		if (itFun == m_resCreateFunMap.end())
		{
			pResource = new Resource(NULL);
		}
		else
		{
			pResource = itFun->second();
		}
		
		pResource->SetResPath(pszRelPath);

		m_resMap[pszRelPath] = pResource;

		return pResource;
	}

	Resource* ResourceSystem::LoadResourceSync(const char* pszRelPath)
	{
		Resource* pRes = DeclareResource(pszRelPath);
		ASSERT(pRes);
		if (pRes == NULL)
			return NULL;

		pRes->LoadSync();

		return pRes;
	}
	
	Resource* ResourceSystem::LoadResource(const char* pszRelPath)
	{
		Resource* pRes = DeclareResource(pszRelPath);
		ASSERT(pRes);
		if (pRes == NULL)
			return NULL;

		pRes->Load();

		return pRes;
	}
	

	void ResourceSystem::RegisterResourceFactory(const char* fileExt,ResourceCreator pResCreator)
	{
		if (fileExt == NULL)
			return;

		ResCreateFunMap::iterator it = m_resCreateFunMap.find(fileExt);
		if (it == m_resCreateFunMap.end())
		{
			m_resCreateFunMap[fileExt] = pResCreator;
		}
	}

	void ResourceSystem::UnregisterResourceFactory(const char* fileExt,ResourceCreator pResCreator)
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

