#include "ResourceSystem.h"

namespace ma
{
	typedef std::map<std::string,ResourcePtr> ResourceMap;
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

	ResourcePtr ResourceSystem::DeclareResource(const char* pszRelPath)
	{
		if (pszRelPath == NULL)
			return NULL;

		ResourceMap::iterator itRes = _resMap.find(pszRelPath);
		if (itRes != _resMap.end())
			return itRes->second;

		std::string fileExt = StringUtil::getFileExt(pszRelPath);
		ResCreateFunMap::iterator itFun = _resCreateFunMap.find(fileExt);
		if (itFun == _resCreateFunMap.end())
			return NULL;
	
		Resource* pResource = itFun->second();
		pResource->SetResPath(pszRelPath);

		_resMap[pszRelPath] = pResource;

		return pResource;
	}
	
	ResourcePtr ResourceSystem::LoadResourceSync(const char* pszRelPath)
	{
		ResourcePtr pRes = DeclareResource(pszRelPath);
		ASSERT(pRes);
		if (pRes == NULL)
			return NULL;

		pRes->LoadSync();

		return pRes;
	}
	
	ResourcePtr ResourceSystem::LoadResourceASync(const char* pszRelPath,EventListener* pCallBack)
	{
		ResourcePtr pRes = DeclareResource(pszRelPath);
		ASSERT(pRes);
		if (pRes == NULL)
			return NULL;

		ResData dataObj;
		dataObj.m_pRes = pRes;
		dataObj.m_pCallBack = pCallBack;

		gpDataThread->PushBackDataObj(dataObj);

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

