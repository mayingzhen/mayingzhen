#include "RTTIClass.h"

namespace ma
{
	static ClassManager* gpClassManager = NULL;

	ClassManager* GetClassManager()
	{
		return gpClassManager;
	}

	void SetClassManager(ClassManager* pClassMang)
	{
		gpClassManager = pClassMang;
	}

	ClassManager::ClassManager()
	{

	}

	void ClassManager::Shutdown()
	{
		ASSERT(m_nameMap.size() == 0,"Some class not clean up properly");
	}

	bool ClassManager::RegisterRTTIClass(RTTIClass* pCls)
	{
		bool bAddOK = false;
		std::map<std::string,RTTIClass*>::iterator nameIter = m_nameMap.find( string( pCls->GetName() ) );
		if (nameIter != m_nameMap.end())
		{
			ASSERT(false);
			LogError("Class name conflict %s",pCls);
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
			LogError("Fail to unregister class %s",pCls->GetName());
		}
		return bOK;

	}

	const RTTIClass* ClassManager::GetClassByName(const char* clsName)
	{
		ClassNameMap::iterator iter = m_nameMap.find(clsName);
		return iter != m_nameMap.end() ? iter->second : NULL;
	}

	RTTIClass::RTTIClass(const char* className,const RTTIClass* pParent)
		:m_className(className)
		,m_pParentNode(pParent)
	{

	}

	const char* RTTIClass::GetName() const
	{
		return m_className.c_str();
	}

	bool RTTIClass::IsA(const RTTIClass* pAnisister) const
	{
		for (const RTTIClass* pCurParent = this; pCurParent != NULL; pCurParent = pCurParent->GetParent() )
		{
			if (pCurParent == pAnisister)
			{
				return true;
			}
		}
		return false;
	}
}

