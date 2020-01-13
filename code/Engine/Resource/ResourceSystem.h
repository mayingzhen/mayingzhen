#pragma once

namespace ma
{
	class Resource;

	template<class T>
	class ResourceSystem
	{
	public:
		ResourceSystem() {}

		~ResourceSystem() {}

		RefPtr<T> CreateResource(const char* pszRelPath,const RES_CALL_BACK& fCallback = nullptr)
		{
			ASSERT(pszRelPath);
			if (pszRelPath == NULL)
				return NULL;

			auto it = m_resMap.find(pszRelPath);
			if (it != m_resMap.end())
			{
				if (fCallback)
				{
					if (it->second->GetResState() <= ResLoadIng)
					{
						it->second->AddCallBack(fCallback);
					}
					else
					{
						fCallback(it->second.get());
					}
				}
				return it->second;
			}

			RefPtr<T> res = new T;
			if (fCallback)
			{
				res->AddCallBack(fCallback);
			}
			if (!res->Load(pszRelPath))
			{
				LogError("can not load file :%s", pszRelPath);
				return NULL;
			}

			m_resMap[pszRelPath] = res;

			return res;
		}

		RefPtr<T> CreateResourceSync(const char* pszRelPath)
		{
			ASSERT(pszRelPath);
			if (pszRelPath == NULL)
				return NULL;

			auto it = m_resMap.find(pszRelPath);
			if (it != m_resMap.end())
			{
				return it->second;
			}

			RefPtr<T> res = new T;
			if (!res->LoadSync(pszRelPath))
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

		void ReLoad()
		{
			for (auto it = m_resMap.begin(); it != m_resMap.end(); ++it)
			{
				it->second->ReLoad();
			}
		}
		
	private:
		typedef std::map<std::string, RefPtr<T> > ResourceMap;

		ResourceMap			m_resMap;
	};
}

