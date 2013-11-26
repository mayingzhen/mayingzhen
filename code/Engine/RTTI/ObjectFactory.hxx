#include "ObjectFactory.h"

namespace ma
{
	template<> ObjectFactoryManager* Singleton<ObjectFactoryManager>::ms_singleton = NULL;

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
		ObjFunFactoryMap::iterator funIter = m_objFunFactoryMap.find(pCls);
		if (funIter != m_objFunFactoryMap.end())
		{
			pObj = funIter->second();
		}
		else
		{
			Log("Object factory not found for type %s",pCls);
		}
		return pObj;
	}

	void ObjectFactoryManager::RegisterObjectDeleteFactory(const char* pCls,ObjectDelete funDelete)
	{
		ObjDeleteFunFactoryMap::iterator iter = m_objDeleteFunMap.find(pCls);
		if (iter == m_objDeleteFunMap.end())
		{
			m_objDeleteFunMap[pCls] = funDelete;
		}
		else
		{
			ASSERT(false);
			Log("Object factory conflict : %s %p",pCls,funDelete);
		}
	}

	void ObjectFactoryManager::DeleteObject(const char* clsName,Object* pObject)
	{
		Object* pObj = NULL;
		ObjDeleteFunFactoryMap::iterator funIter = m_objDeleteFunMap.find(clsName);
		if (funIter != m_objDeleteFunMap.end())
		{
			funIter->second(pObject);
		}
		else
		{
			Log("Object factory not found for type %s",clsName);
		}
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
			Log("Object factory conflict : %s %p",pCls,funArgCreator);
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
			Log("Fail to unregister object factory");
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
			Log("Object factory not found for type %s",pCls);
		}
		return pObj;
	}

}

