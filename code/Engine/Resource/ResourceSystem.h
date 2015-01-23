#ifndef  _RESOURCE_MANAGER__H__
#define  _RESOURCE_MANAGER__H__

namespace ma
{
	class Resource;
	class DataThread;

	typedef Resource* (*ResourceCreator)();

	class ENGINE_API ResourceSystem
	{
	public:
		ResourceSystem();

		void		Init(bool bDataThread);

		void		Shoutdown();

		void		SetDataThreadEnable(bool b);

		void		Update();

		DataThread*	GetDataThread();

		Resource*	DeclareResource(const char* pszRelPath);

		Resource*	LoadResource(const char* pszRelPath); 

		Resource*	LoadResourceSync(const char* pszRelPath);

		void		RegisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);

		void		UnregisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);
		
		template <class T>
		void		ReLoad();	

	private:
		typedef std::map<std::string,ResourcePtr> ResourceMap;
		typedef std::map<std::string,ResourceCreator> ResCreateFunMap;

		ResourceMap			m_resMap;
		ResCreateFunMap		m_resCreateFunMap;

		DataThread*			m_pDataThread;
	};

	ENGINE_API ResourceSystem*	GetResourceSystem();

	ENGINE_API	void			SetResourceSystem(ResourceSystem* pResSystem);

	template <class T>
	T*	DeclareResource(const char* pszRelPath)
	{
		Resource* pRes = GetResourceSystem()->DeclareResource(pszRelPath);
		ASSERT(pRes);

		T* pTypeRes = SafeCast<T>(pRes);
		ASSERT(pTypeRes);
		return pTypeRes;
	}

	template <class T>
	T*	LoadResource(const char* pszRelPath)
	{
		Resource* pRes = GetResourceSystem()->LoadResource(pszRelPath);
		ASSERT(pRes);
	
		T* pTypeRes = SafeCast<T>(pRes);
		ASSERT(pTypeRes);
		return pTypeRes;
	}

	template <class T>
	T*	LoadResourceSync(const char* pszRelPath)
	{
		Resource* pRes = GetResourceSystem()->LoadResourceSync(pszRelPath);
		ASSERT(pRes);

		T* pTypeRes = SafeCast<T>(pRes);
		ASSERT(pTypeRes);
		return pTypeRes;
	}

	template <class T>
	void ResourceSystem::ReLoad()
	{
		ResourceMap::iterator it = m_resMap.begin();
		for (; it != m_resMap.end(); ++it)
		{
			Resource* pRes = it->second.get();
			
			T* pTypeRes = SafeCast<T>(pRes); 
			if (pTypeRes)
			{
				pTypeRes->Reload();
			}
		}
	}
}


#endif