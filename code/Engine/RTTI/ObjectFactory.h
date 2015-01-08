#ifndef __ObjectFactory_H__
#define __ObjectFactory_H__

#include "Engine/RTTI/Object.h"

namespace ma
{
	typedef Object* (*ObjectCreator)();
	typedef Object* (*ObjectCreatorArg)(void* arg);

	typedef void	(*ObjectDelete)(Object* pObject);

	class ENGINE_API ObjectFactoryManager
	{
	public:

		virtual void Shutdown();

		virtual void RegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		virtual void RegisterObjectFactory(const char* pCls,ObjectCreatorArg funArgCreator);

		virtual void UnRegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		virtual void UnRegisterObjectFactory(const char* pCls,ObjectCreatorArg funArgCreator);

		virtual Object* CreateObject(const char* clsName);

		virtual Object*	CreateObjectArg(const char* claName,void* arg);

	private:
		typedef std::map<std::string,ObjectCreator> ObjFunFactoryMap;
		typedef std::map<std::string,ObjectCreatorArg> ObjFunArgFactoryMap;

		ObjFunFactoryMap	m_objFunFactoryMap;
		ObjFunArgFactoryMap	m_objFunArgFactoryMap;
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

	template <class T>
	T*	CreateObjectArg(const char* pszClassName,void* pArg)
	{
		Object* pObject = GetObjectFactoryManager()->CreateObjectArg(pszClassName,pArg);
		ASSERT(pObject);

		T* pTypeObject = SafeCast<T>(pObject);
		ASSERT(pTypeObject);
		return pTypeObject;
	}

	ObjectFactoryManager* GetObjectFactoryManager();

	void SetObjectFactoryManager(ObjectFactoryManager* pClassMang);

}

#endif //__ObjectFactory_H__
