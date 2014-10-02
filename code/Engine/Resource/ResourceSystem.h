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
		void		Init();

		void		ShoutDown();

		void		Update();

		DataThread*	GetDataThread();

		ResourcePtr	DeclareResource(const char* pszRelPath);

		ResourcePtr	LoadResourceSync(const char* pszRelPath); 

		ResourcePtr	LoadResourceASync(const char* pszRelPath,EventListener* pCallBack);

		static void	RegisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);

		static void	UnregisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);

	};

	ENGINE_API ResourceSystem*	GetResourceSystem();

	ENGINE_API	void			SetResourceSystem(ResourceSystem* pResSystem);

	template <class T>
	RefPtr<T>	DeclareResource(const char* pszRelPath)
	{
		RefPtr<Resource> pRes = GetResourceSystem()->DeclareResource(pszRelPath);
		ASSERT(pRes);

		T* pTypeRes = SafeCast<T>(pRes.get());
		ASSERT(pTypeRes);
		return pTypeRes;
	}

	template <class T>
	RefPtr<T>	LoadResourceSync(const char* pszRelPath)
	{
		RefPtr<Resource> pRes = GetResourceSystem()->LoadResourceSync(pszRelPath);
		ASSERT(pRes);

		T* pTypeRes = SafeCast<T>(pRes.get());
		ASSERT(pTypeRes);
		return pTypeRes;
	}

	template <class T>
	RefPtr<T>	LoadResourceASync(const char* pszRelPath,EventListener* pCallBack)
	{
		RefPtr<Resource> pRes = GetResourceSystem()->LoadResourceASync(pszRelPath,pCallBack);
		ASSERT(pRes);

		T* pTypeRes = SafeCast<T>(pRes.get());
		ASSERT(pTypeRes);
		return pTypeRes;
	}
}


#endif