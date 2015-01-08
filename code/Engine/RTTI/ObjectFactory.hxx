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

	Object* ObjectFactoryManager::CreateObject(const char* pCls)
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


	void ObjectFactoryManager::RegisterObjectFactory(const char* pCls,ObjectCreatorArg funArgCreator)
	{
		ObjFunArgFactoryMap::iterator iter = m_objFunArgFactoryMap.find(pCls);
		if (iter == m_objFunArgFactoryMap.end())
		{
			m_objFunArgFactoryMap[pCls] = funArgCreator;
		}
		else
		{
			ASSERT(false);
			LogError("Object factory conflict : %s %p",pCls,funArgCreator);
		}
	}

	void ObjectFactoryManager::UnRegisterObjectFactory(const char* pCls,ObjectCreatorArg funArgCreator)
	{
		ObjFunArgFactoryMap::iterator iter = m_objFunArgFactoryMap.find(pCls);
		if (iter != m_objFunArgFactoryMap.end() && iter->second == funArgCreator)
		{
			m_objFunArgFactoryMap.erase(iter);
		}
		else
		{
			LogError("Fail to unregister object factory");
		}
	}

	Object*	ObjectFactoryManager::CreateObjectArg(const char* pCls,void* arg)
	{
		Object* pObj = NULL;
		ObjFunArgFactoryMap::iterator funIter = m_objFunArgFactoryMap.find(pCls);
		if (funIter != m_objFunArgFactoryMap.end())
		{
			pObj = funIter->second(arg);
		}
		else
		{
			ASSERT(false);
			LogError("Object factory not found for type %s",pCls);
		}
		return pObj;
	}

}

