#include "IntenalCall.h"
#include "ScriptObject.h"

using namespace ma;

static MonoObject* MonoGameObject_GetScript(int gameObjPtr,MonoString* pScriptNameM)
{
	GameObject* pGameObj = (GameObject*)gameObjPtr;
	std::string strScriptName = mono_string_to_utf8(pScriptNameM);

	IScriptObject* pScriptObj = NULL;
	for (UINT nCnt = 0; nCnt < pGameObj->GetComponentNumber(); ++nCnt)
	{
		Component* pComp = pGameObj->GetComponentByIndex(nCnt);
		pScriptObj = SafeCast<IScriptObject>(pComp);
		if (pScriptObj == NULL)
			continue;

		if ( stricmp(pScriptObj->GetName(),strScriptName.c_str()) == 0)
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
	mono_add_internal_call("EngineInternal::MonoGameObject_GetScript(int,string)",MonoGameObject_GetScript);

	mono_add_internal_call("EngineInternal::AddCollisionListener(int,int)",AddCollisionListener);

	mono_add_internal_call("EngineInternal::RemoveCollisionListener(int,int)",RemoveCollisionListener);


}


