#include "ObjectFactory.h"

namespace ma
{
	static ObjectFactoryManager* gpObjectFactoryManager = NULL;

	ObjectFactoryManager* GetObjectFactoryManager()
	{
		return gpObjectFactoryManager;
	}

	void SetObjectFactoryManager(ObjectFactoryManager* pClassMang)
	{
		gpObjectFactoryManager = pClassMang;
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
			ASSERT(false);
			LogError("Object factory conflict : %s %p",pCls,funCreator);
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
			LogError("Fail to unregister object factory");
		}
	}

	RefPtr<Object> ObjectFactoryManager::CreateObject(const char* pCls)
	{
		Object* pObj = NULL;
		ObjFunFactoryMap::iterator funIter = m_objFunFactoryMap.find(pCls);
		if (funIter != m_objFunFactoryMap.end())
		{
			pObj = funIter->second();
		}
		else
		{
			LogError("Object factory not found for type %s",pCls);
		}
		return pObj;
	}
}

