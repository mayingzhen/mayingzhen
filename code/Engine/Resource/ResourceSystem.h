#ifndef  _RESOURCE_MANAGER__H__
#define  _RESOURCE_MANAGER__H__

namespace ma
{
	class Resource;

	template<class T>
	class ResourceSystem
	{
	public:
		ResourceSystem() {}

		~ResourceSystem() {}

		RefPtr<T> CreateResource(const char* pszRelPath)
		{
			ASSERT(pszRelPath);
			if (pszRelPath == NULL)
				return NULL;

			ResourceMap::iterator itRes = m_resMap.find(pszRelPath);
			if (itRes != m_resMap.end())
				return itRes->second;

			RefPtr<T> res = new T;
			if (!res->Load(pszRelPath))
			{
				LogError("can not load file :%s", pszRelPath);
				return NULL;
			}

			m_resMap[pszRelPath] = res;

			return res;
		}

		void Clear()
		{
			m_resMap.clear();
		}
		
	private:
		typedef std::map<std::string, RefPtr<T> > ResourceMap;

		ResourceMap			m_resMap;
	};
}


#endif