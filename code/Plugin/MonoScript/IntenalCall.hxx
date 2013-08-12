#include "IntenalCall.h"
#include "ScriptObject.h"

using namespace ma;

static MonoObject* MonoGameObject_GetScript(int gameObjPtr,MonoString* pScriptNameM)
{
	GameObject* pGameObj = (GameObject*)gameObjPtr;
	std::string strScriptName = mono_string_to_utf8(pScriptNameM);

	IScriptObject* pRetScriptObject = NULL;
	for (UINT nCnt = 0; nCnt < pGameObj->GetComponentNumber(); ++nCnt)
	{
		Component* pComp = pGameObj->GetComponentByIndex(nCnt);
		ScriptComponent* pScriptComp = SafeCast<ScriptComponent>(pComp);
		if (pScriptComp == NULL)
			continue;

		pRetScriptObject = pScriptComp->GetScriptObject();
		if (pRetScriptObject == NULL)
			continue;

		if ( stricmp(pRetScriptObject->GetName(),strScriptName.c_str()) == 0)
		{
			break;
		}
	}

	ScriptObject* pMonoScriptObject = (ScriptObject*)pRetScriptObject;
	return NULL != pMonoScriptObject ? pMonoScriptObject->GetMonoObject() : NULL;
}

static void AddCollisionListener(int scriptObjPtr, int gameObjPtr)
{

}

static void RemoveCollisionListener(int scriptObjPtr, int gameObjPtr)
{

}



void MonoInternalInit()
{	
	mono_add_internal_call("EngineInternal::MonoGameObject_GetScript(int,string)",MonoGameObject_GetScript);

	mono_add_internal_call("EngineInternal::AddCollisionListener(int,int)",AddCollisionListener);

	mono_add_internal_call("EngineInternal::RemoveCollisionListener(int,int)",RemoveCollisionListener);


}


