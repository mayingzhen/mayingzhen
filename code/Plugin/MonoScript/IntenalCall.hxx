#include "IntenalCall.h"
#include "ScriptObject.h"

using namespace ma;

static MonoObject* MonoGameObject_GetScript(int gameObjPtr,MonoString* pScriptNameM)
{
	SceneNode* pGameObj = (SceneNode*)gameObjPtr;
	std::string strScriptName = mono_string_to_utf8(pScriptNameM);

	ScriptObject* pScriptObj = NULL;
	for (uint32_t nCnt = 0; nCnt < pGameObj->GetComponentNumber(); ++nCnt)
	{
		RefPtr<Component> pComp = pGameObj->GetComponentByIndex(nCnt);
		pScriptObj = dynamic_cast<ScriptObject*>(pComp.get());
		if (pScriptObj == NULL)
			continue;

		if ( strcmp(pScriptObj->GetName(),strScriptName.c_str()) == 0)
		{
			break;
		}
	}

	ScriptObject* pMonoScriptObject = (ScriptObject*)pScriptObj;
	return pMonoScriptObject ? pMonoScriptObject->GetMonoObject() : NULL;
}

static void AddCollisionListener(int scriptObjPtr, int gameObjPtr)
{

}

static void RemoveCollisionListener(int scriptObjPtr, int gameObjPtr)
{

}



void MonoInternalInit()
{	
	mono_add_internal_call("EngineInternal::MonoGameObject_GetScript(int,string)",(const void*)MonoGameObject_GetScript);

	mono_add_internal_call("EngineInternal::AddCollisionListener(int,int)",(const void*)AddCollisionListener);

	mono_add_internal_call("EngineInternal::RemoveCollisionListener(int,int)",(const void*)RemoveCollisionListener);


}


