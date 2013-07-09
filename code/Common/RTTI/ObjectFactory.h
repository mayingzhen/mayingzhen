#ifndef __ObjectFactory_H__
#define __ObjectFactory_H__

#include "Common/Singleton.h"
#include "Common/RTTI/Object.h"

namespace ma
{
	typedef Object* (*ObjectCreator)();
	typedef Object* (*ObjectCreatorArg)(void* arg);

	class COMMON_API ObjectFactoryManager : public Singleton<ObjectFactoryManager>
	{
		typedef std::map<std::string,ObjectCreator> ObjFunFactoryMap;
		typedef std::map<std::string,ObjectCreatorArg> ObjFunArgFactoryMap;
		ObjFunFactoryMap	m_objFunFactoryMap;
		ObjFunArgFactoryMap	m_objFunArgFactoryMap;

	public:

		virtual void Shutdown();

		virtual void RegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		virtual void RegisterObjectFactory(const char* pCls,ObjectCreatorArg funArgCreator);

		virtual void UnRegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		virtual void UnRegisterObjectFactory(const char* pCls,ObjectCreatorArg funArgCreator);

		virtual Object* CreateObject(const char* clsName);

		virtual Object*	CreateObjectArg(const char* claName,void* arg);
	};
}

#endif //__ObjectFactory_H__
