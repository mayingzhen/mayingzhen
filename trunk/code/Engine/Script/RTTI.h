#ifndef __IScriptSystemRTTI_H__
#define __IScriptSystemRTTI_H__

namespace ma
{
	Object*	Create_ScriptObject(void* arg) {return GetScriptSystem()->CreateScriptObject( (GameObject*)arg );}
	void	Delete_ScriptObject(Object* pObj) {GetScriptSystem()->DeleteScriptObject( (IScriptObject*)pObj );}

	void IScriptSystemRTTIInit()
	{
#define RTTI_DECL(ClassType,showType) \
	ClassType::StaticInitClass(); \
	ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##showType);  \
	ObjectFactoryManager::GetInstance().RegisterObjectDeleteFactory(#ClassType,Delete_##showType);

		RTTI_DECL(IScriptObject,ScriptObject);
#undef RTTI_DECL
	}


	void IScriptSystemRTTIShoutdown()
	{
#define RTTI_DECL(ClassType,showType) \
	ClassType::StaticInitClass(); \
	ObjectFactoryManager::GetInstance().UnRegisterObjectFactory(#ClassType,Create_##showType);  \
	//ObjectFactoryManager::GetInstance().UnRegisterObjectDeleteFactory(#ClassType,Delete_##showType);

		RTTI_DECL(IScriptObject,ScriptObject);
#undef RTTI_DECL
	}
}



#endif