#include "Common/RTTI/RTTIClass.h"

namespace ma
{
	template<> ClassManager* Singleton<ClassManager>::ms_singleton = NULL;

	ClassManager::ClassManager()
	{

	}

	void ClassManager::Shutdown()
	{
		ASSERT_MSG(m_nameMap.size() == 0,"Some class not clean up properly");
	}

	bool ClassManager::RegisterRTTIClass(RTTIClass* pCls)
	{
		bool bAddOK = false;
		std::map<std::string,RTTIClass*>::iterator nameIter = m_nameMap.find(pCls->GetName());
		if (nameIter != m_nameMap.end())
		{
			ASSERT(false);
			Log("Class name conflict %s",pCls);
		}
		else 
		{
			bAddOK = true;
			m_nameMap[pCls->GetName()] = pCls;
		}

		return bAddOK;
	}

	bool ClassManager::UnRegisterRTTIClass(RTTIClass* pCls)
	{
		bool bOK = false;
		ClassNameMap::iterator nameIter = m_nameMap.find(pCls->GetName());
		if (nameIter != m_nameMap.end() && pCls == nameIter->second)
		{
			m_nameMap.erase(nameIter);
			bOK = true;
		}
		else
		{
			Log("Fail to unregister class %s",pCls->GetName());
		}
		return bOK;

	}

	const RTTIClass* ClassManager::GetClassByName(const char* clsName)
	{
		ClassNameMap::iterator iter = m_nameMap.find(clsName);
		return iter != m_nameMap.end() ? iter->second : NULL;
	}
}

