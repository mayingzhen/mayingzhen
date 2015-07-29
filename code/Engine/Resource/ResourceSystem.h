#ifndef  _RESOURCE_MANAGER__H__
#define  _RESOURCE_MANAGER__H__

namespace ma
{
	class Resource;
	class DataThread;

	typedef Resource* (*ResourceCreator)();

	class ResourceSystem
	{
	public:
		ResourceSystem();

		void		Init(bool bDataThread);

		void		Shoutdown();

		void		SetDataThreadEnable(bool b);

		void		Update();

		DataThread*	GetDataThread();

		Resource*	FindResource(const char* pszRelPath);

		Resource*	DeclareResource(const char* pszRelPath);

		Resource*	LoadResource(const char* pszRelPath); 

		Resource*	LoadResourceSync(const char* pszRelPath);

		void		RegisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);

		void		UnregisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);
		
		template <class T>
		void		ReLoad();	

	private:
		typedef std::map<std::string, RefPtr<Resource> > ResourceMap;
		typedef std::map<std::string,ResourceCreator> ResCreateFunMap;

		ResourceMap			m_resMap;
		ResCreateFunMap		m_resCreateFunMap;

		DataThread*			m_pDataThread;
	};

	ResourceSystem*	GetResourceSystem();

	void			SetResourceSystem(ResourceSystem* pResSystem);

	template <class T>
	T*	DeclareResource(const char* pszRelPath)
	{
		Resource* pRes = GetResourceSystem()->DeclareResource(pszRelPath);
		ASSERT(pRes);

		T* pTypeRes = dynamic_cast<T*>(pRes);
		ASSERT(pTypeRes);
		return pTypeRes;
	}

	template <class T>
	T*	FindResource(const char* pszRelPath)
	{
		Resource* pRes = GetResourceSystem()->FindResource(pszRelPath);
		if (pRes == NULL)
			return NULL;

		T* pTypeRes = dynamic_cast<T*>(pRes);
		ASSERT(pTypeRes);
		return pTypeRes;
	}


	template <class T>
	T*	LoadResource(const char* pszRelPath)
	{
		Resource* pRes = GetResourceSystem()->LoadResource(pszRelPath);
		ASSERT(pRes);
	
		T* pTypeRes = dynamic_cast<T*>(pRes);
		ASSERT(pTypeRes);
		return pTypeRes;
	}

	template <class T>
	T*	LoadResourceSync(const char* pszRelPath)
	{
		Resource* pRes = GetResourceSystem()->LoadResourceSync(pszRelPath);
		ASSERT(pRes);

		T* pTypeRes = dynamic_cast<T*>(pRes);
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
			
			T* pTypeRes = dynamic_cast<T*>(pRes); 
			if (pTypeRes)
			{
				pTypeRes->Reload();
			}
		}
	}
}


#endif