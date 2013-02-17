#include "Common/RTTI/ObjectFactory.h"

namespace ma
{
	ObjectFactoryManager* Singleton<ObjectFactoryManager>::ms_singleton = NULL;

	void ObjectFactoryManager::Shutdown()
	{

	}

	void ObjectFactoryManager::RegisterObjectFactory(const char* pCls,ObjectCreator funCreator)
	{
		ObjFunFactoryMap::iterator iter = m_objFunFactoryMap.find(pCls);
		if (iter == m_objFunFactoryMap.end())
		{
			m_objFunFactoryMap[pCls] = funCreator;
		}
		else
		{
			Log("Object factory conflict : %s %p",pCls,funCreator);
		}
	}

	void ObjectFactoryManager::UnRegisterObjectFactory(const char* pCls,ObjectCreator funCreator)
	{
		ObjFunFactoryMap::iterator iter = m_objFunFactoryMap.find(pCls);
		if (iter != m_objFunFactoryMap.end() && iter->second == funCreator)
		{
			m_objFunFactoryMap.erase(iter);
		}
		else
		{
			Log("Fail to unregister object factory");
		}
	}

	Object* ObjectFactoryManager::CreateObject(const char* pCls)
	{
		Object* pObj = NULL;
		//ObjFunFactoryMap::iterator iter = m_objFunFactoryMap.find(pCls);
		//if (iter != m_objFunFactoryMap.end())
		//{
			ObjFunFactoryMap::iterator funIter = m_objFunFactoryMap.find(pCls);
			if (funIter != m_objFunFactoryMap.end())
			{
				pObj = funIter->second();
			}
			else
			{
				Log("Object factory not found for type %s",pCls);
			}
		//}
		return pObj;
	}

// 	Object* ObjectFactoryManager::CreateObject(const char* clsName)
// 	{
// 		Object* pObj = NULL;
// 		const Class* pCls = ClassManager::GetInstance().GetClassByName(clsName);
// 		if (NULL != pCls)
// 		{
// 			pObj = CreateObject(pCls);
// 		}
// 		else
// 		{
// 			Log("Class not found %s",clsName);
// 		}
// 		return pObj;
// 	}
}

