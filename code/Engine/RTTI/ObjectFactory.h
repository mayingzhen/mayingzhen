#ifndef __ObjectFactory_H__
#define __ObjectFactory_H__

#include "Engine/RTTI/Object.h"

namespace ma
{
	typedef RefPtr<Object> (*ObjectCreator)();

	class ObjectFactoryManager
	{
	public:

		void	RegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		void	UnRegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		RefPtr<Object> CreateObject(const char* clsName);
		
		void Clear();

	private:
		typedef std::map<std::string,ObjectCreator> ObjFunFactoryMap;
	
		ObjFunFactoryMap	m_objFunFactoryMap;
	};
	
	extern ObjectFactoryManager* g_pObjectFactoryManager;
	ObjectFactoryManager* GetObjectFactoryManager();

	template <class T>
	RefPtr<T>	CreateObject(const char* pszClassName = NULL)
	{
		RefPtr<Object> pObject;
		if (pszClassName)
			pObject = GetObjectFactoryManager()->CreateObject(pszClassName);
		else
			pObject = GetObjectFactoryManager()->CreateObject( T::StaticGetClassName() );

		ASSERT(pObject);

		RefPtr<T> pTypeObject = dynamic_cast<T*>(pObject.get());
		ASSERT(pTypeObject);
		return pTypeObject;
	}

	#define REGISTER_OBJECT(className, createFunction) GetObjectFactoryManager()->RegisterObjectFactory(className::StaticGetClassName(),(ObjectCreator)createFunction);

}

#endif //__ObjectFactory_H__
