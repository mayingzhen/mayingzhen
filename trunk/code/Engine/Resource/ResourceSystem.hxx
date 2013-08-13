#include "ResourceSystem.h"

namespace ma
{
	typedef std::map<std::string,Resource*> ResourceMap;
	typedef std::map<std::string,ResourceCreator> ResCreateFunMap;

	static ResourceMap		_resMap;
	static ResCreateFunMap	_resCreateFunMap;

	static	DataThread*		gpDataThread = NULL;
	static  ResourceSystem*	gpResourceSystem = NULL;

	ResourceSystem*	GetResourceSystem()
	{
		return gpResourceSystem;
	}


	void SetResourceSystem(ResourceSystem* pResSystem)
	{
		gpResourceSystem = pResSystem;
	}

	void ResourceSystem::Init()
	{
		gpDataThread = new DataThread();
		gpDataThread->Start();
	}

	void ResourceSystem::ShoutDown()
	{
		gpDataThread->Stop();
		SAFE_DELETE(gpDataThread);
	}

	void ResourceSystem::Update()
	{
		gpDataThread->Process();
	}

	DataThread*	ResourceSystem::GetDataThread()
	{	
		return gpDataThread;
	}

	Resource* ResourceSystem::DeclareResource(const char* pszRelPath)
	{
		if (pszRelPath == NULL)
			return NULL;

		ResourceMap::iterator itRes = _resMap.find(pszRelPath);
		if (itRes != _resMap.end())
			return itRes->second;

		std::string fileExt = FileSystem::getExtension(pszRelPath);
		ResCreateFunMap::iterator itFun = _resCreateFunMap.find(fileExt);
		if (itFun == _resCreateFunMap.end())
			return NULL;
	
		Resource* pResource = itFun->second();
		pResource->SetResPath(pszRelPath);

		_resMap[pszRelPath] = pResource;

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

	Resource* ResourceSystem::LoadResourceASync(const char* pszRelPath)
	{
		Resource* pRes = DeclareResource(pszRelPath);
		ASSERT(pRes);
		if (pRes == NULL)
			return NULL;

		gpDataThread->PushBackDataObj((IDataObj*)pRes);

		return pRes;	
	}

	void ResourceSystem::RegisterResourceFactory(const char* fileExt,ResourceCreator pResCreator)
	{
		if (fileExt == NULL)
			return;

		ResCreateFunMap::iterator it = _resCreateFunMap.find(fileExt);
		if (it == _resCreateFunMap.end())
		{
			_resCreateFunMap[fileExt] = pResCreator;
		}
	}

	void ResourceSystem::UnregisterResourceFactory(const char* fileExt,ResourceCreator pResCreator)
	{
		if (fileExt == NULL)
			return;

		ResCreateFunMap::iterator it = _resCreateFunMap.find(fileExt);
		if (it != _resCreateFunMap.end() && it->second == pResCreator)
		{
			_resCreateFunMap.erase(it);
		}
	}

}

