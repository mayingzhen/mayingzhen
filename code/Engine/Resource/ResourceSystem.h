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

		Resource*	DeclareResource(const char* pszRelPath);

		Resource*	LoadResourceSync(const char* pszRelPath); 

		Resource*	LoadResourceASync(const char* pszRelPath);

		static void	RegisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);

		static void	UnregisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);

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
	T*	LoadResourceSync(const char* pszRelPath)
	{
		Resource* pRes = GetResourceSystem()->LoadResourceSync(pszRelPath);
		ASSERT(pRes);

		T* pTypeRes = SafeCast<T>(pRes);
		ASSERT(pTypeRes);
		return pTypeRes;
	}

	template <class T>
	T*	LoadResourceASync(const char* pszRelPath)
	{
		Resource* pRes = GetResourceSystem()->LoadResourceASync(pszRelPath);
		ASSERT(pRes);

		T* pTypeRes = SafeCast<T>(pRes);
		ASSERT(pTypeRes);
		return pTypeRes;
	}
}


#endif