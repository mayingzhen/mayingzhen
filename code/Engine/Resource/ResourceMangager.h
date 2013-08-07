#ifndef  _RESOURCE_MANAGER__H__
#define  _RESOURCE_MANAGER__H__

namespace ma
{
	class Resource;
	class DataThread;

	typedef Resource* (*ResourceCreator)();

	class ENGINE_API ResourceManager
	{
	public:
		static Resource*	DeclareResource(const char* pszRelPath);

		static void			RegisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);

		static void			UnregisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);

	private:
		typedef std::map<std::string,Resource*> ResourceMap;
		typedef std::map<std::string,ResourceCreator> ResCreateFunMap;

		static ResourceMap		m_resMap;
		
		static ResCreateFunMap	m_resCreateFunMap;
	};

	template <class T>
	T*	DeclareResource(const char* pszRelPath)
	{
		Resource* pRes = ResourceManager::DeclareResource(pszRelPath);
		ASSERT(pRes);

		T* pTypeRes = SafeCast<T>(pRes);
		ASSERT(pTypeRes);
		return pTypeRes;
	}
}


#endif