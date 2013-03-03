#include "Common/RTTI/Class.h"

namespace ma
{
	template<> ClassManager* Singleton<ClassManager>::ms_singleton = NULL;

	ClassManager::ClassManager()
	{

	}

	void ClassManager::Shutdown()
	{
		SSERT_MSG(m_nameMap.size() == 0,"Some class not clean up properly");
	}

	bool ClassManager::RegisterClass(Class* pCls)
	{
		bool bAddOK = false;
		ClassNameMap::iterator nameIter = m_nameMap.find(pCls->GetName());
		//ClassIDMap::iterator idIter = m_idMap.find(pCls->GetClassID());

// 		if (idIter != m_idMap.end())
// 		{
// 			Log("\nClass ID conflict <%s,%s> id = %d "
// 				,idIter->second->GetName()
// 				,pCls->GetName()
// 				,pCls->GetClassID());
// 		}
// 		else 
		if (nameIter != m_nameMap.end())
		{
			Log("\nClass name conflict %s",pCls);
		}else {
			bAddOK = true;
			m_nameMap[pCls->GetName()] = pCls;
			//m_idMap[pCls->GetClassID()] = pCls;
		}

		return bAddOK;
	}

	bool ClassManager::UnRegisterClass(Class* pCls)
	{
		bool bOK = false;
		ClassNameMap::iterator nameIter = m_nameMap.find(pCls->GetName());
		//ClassIDMap::iterator idIter = m_idMap.find(pCls->GetClassID());
		if (nameIter != m_nameMap.end() && 
			/*idIter != m_idMap.end() &&*/ pCls == nameIter->second)
		{
			//m_idMap.erase(idIter);
			m_nameMap.erase(nameIter);
			bOK = true;
		}else{
			Log("Fail to unregister class %s",pCls->GetName());
		}
		return bOK;

	}

	const Class* ClassManager::GetClassByName(const char* clsName)
	{
		ClassNameMap::iterator iter = m_nameMap.find(clsName);
		return iter != m_nameMap.end() ? iter->second : NULL;
	}

	//const Class* ClassManager::GetClassById(UINT nClsID)
	//{
	//	ClassIDMap::iterator iter = m_idMap.find(nClsID);
	//	return iter != m_idMap.end() ? iter->second : NULL;
	//}
}

