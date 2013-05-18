#ifndef __ObjectFactory_H__
#define __ObjectFactory_H__

#include "Common/Singleton.h"
#include "Common/RTTI/Object.h"

namespace ma
{
	typedef Object* (*ObjectCreator)();

	class COMMON_API ObjectFactoryManager : public Singleton<ObjectFactoryManager>
	{
		typedef std::map<std::string,ObjectCreator> ObjFunFactoryMap;
		ObjFunFactoryMap m_objFunFactoryMap;

	public:

		virtual void Shutdown();

		virtual void RegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		virtual void UnRegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		//virtual Object* CreateObject(const Class* pCls);

		virtual Object* CreateObject(const char* clsName);

		template<class T>
		T* CreateObjectTyped()
		{
			Object* pObj = CreateObject(T::StaticGetClass());
			T* pT = SafeCast<T>(pObj);
			ASSERT(pObj == pT);
			return pT;
		}
	};
}

#endif //__ObjectFactory_H__
