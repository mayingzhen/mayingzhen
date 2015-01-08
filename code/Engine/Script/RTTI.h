#ifndef __IScriptSystemRTTI_H__
#define __IScriptSystemRTTI_H__

namespace ma
{
	Object*	Create_ScriptObject(void* arg) {return GetScriptSystem()->CreateScriptObject( (SceneNode*)arg );}

	void IScriptSystemRTTIInit()
	{
#define RTTI_DECL(ClassType,showType) \
		GetObjectFactoryManager()->RegisterObjectFactory(#ClassType,Create_##showType);  

		RTTI_DECL(IScriptObject,ScriptObject);
#undef RTTI_DECL
	}


	void IScriptSystemRTTIShoutdown()
	{
#define RTTI_DECL(ClassType,showType) \
	GetObjectFactoryManager()->UnRegisterObjectFactory(#ClassType,Create_##showType);  

		RTTI_DECL(IScriptObject,ScriptObject);
#undef RTTI_DECL
	}
}



#endif