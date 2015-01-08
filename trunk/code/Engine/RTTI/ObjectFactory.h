#ifndef __ObjectFactory_H__
#define __ObjectFactory_H__

#include "Engine/RTTI/Object.h"

namespace ma
{
	typedef Object* (*ObjectCreator)();

	class ENGINE_API ObjectFactoryManager
	{
	public:

		virtual void RegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		virtual void UnRegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		virtual Object* CreateObject(const char* clsName);

	private:
		typedef std::map<std::string,ObjectCreator> ObjFunFactoryMap;
	
		ObjFunFactoryMap	m_objFunFactoryMap;
	};

	template <class T>
	T*	CreateObject(const char* pszClassName)
	{
		Object* pObject = GetObjectFactoryManager()->CreateObject(pszClassName);
		ASSERT(pObject);
		
		T* pTypeObject = SafeCast<T>(pObject);
 		ASSERT(pTypeObject);
 		return pTypeObject;
	}

	ObjectFactoryManager* GetObjectFactoryManager();

	void SetObjectFactoryManager(ObjectFactoryManager* pClassMang);

}

#endif //__ObjectFactory_H__
