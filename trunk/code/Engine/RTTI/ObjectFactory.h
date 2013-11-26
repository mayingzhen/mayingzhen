#ifndef __ObjectFactory_H__
#define __ObjectFactory_H__

#include "Common/Singleton.h"
#include "Engine/RTTI/Object.h"

namespace ma
{
	typedef Object* (*ObjectCreator)();
	typedef Object* (*ObjectCreatorArg)(void* arg);

	typedef void	(*ObjectDelete)(Object* pObject);

	class ENGINE_API ObjectFactoryManager : public Singleton<ObjectFactoryManager>
	{
		typedef std::map<std::string,ObjectCreator> ObjFunFactoryMap;
		typedef std::map<std::string,ObjectCreatorArg> ObjFunArgFactoryMap;
		typedef std::map<std::string,ObjectDelete> ObjDeleteFunFactoryMap;
		ObjFunFactoryMap	m_objFunFactoryMap;
		ObjFunArgFactoryMap	m_objFunArgFactoryMap;
		ObjDeleteFunFactoryMap m_objDeleteFunMap;

	public:

		virtual void Shutdown();

		virtual void RegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		virtual void RegisterObjectFactory(const char* pCls,ObjectCreatorArg funArgCreator);

		virtual void UnRegisterObjectFactory(const char* pCls,ObjectCreator funCreator);

		virtual void UnRegisterObjectFactory(const char* pCls,ObjectCreatorArg funArgCreator);

		virtual void RegisterObjectDeleteFactory(const char* pCls,ObjectDelete funDelete);

		virtual Object* CreateObject(const char* clsName);

		virtual Object*	CreateObjectArg(const char* claName,void* arg);

		virtual void	DeleteObject(const char* clsName,Object* pObject);

	};
}

#endif //__ObjectFactory_H__
