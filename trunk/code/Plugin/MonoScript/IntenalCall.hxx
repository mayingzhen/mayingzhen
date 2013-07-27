#include "IntenalCall.h"
#include "ScriptObject.h"

using namespace ma;

static MonoObject* MonoGameObject_GetScript(int goPtr,MonoString* pScriptNameM)
{
	GameObject* pGameObj = (GameObject*)goPtr;
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


void MonoInternalInit()
{	
	mono_add_internal_call("EngineInternal::MonoGameObject_GetScript(int,string)",MonoGameObject_GetScript);

}


