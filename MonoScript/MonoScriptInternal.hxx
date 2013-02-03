#include <MonoScript/MonoScriptIntenal.h>
#include "MonoScript/MonoScriptObject.h"

#include "MonoScript/MonoUtil.h"

namespace ma
{

// 		typedef std::map<S3AName,S3AName> MonoTypeMap;
// 		MonoTypeMap gs_monoTypeMap;
// 
// 		void Init()
// 		{
// 			
// 			gs_monoTypeMap["System.Int32"] = "int";
// 			gs_monoTypeMap["System.UInt32"] = "uint";
// 			gs_monoTypeMap["System.Single"] = "float";
// 			
// 		}
// 
// 		const char* MapTypeMonoToCRT(const char* pszMonoType)
// 		{
// 			MonoTypeMap::iterator iter = gs_monoTypeMap.find(pszMonoType);
// 			return iter != gs_monoTypeMap.end() ? iter->second.GetName() : pszMonoType;
// 		}
// 
// 	}
// 
// 	static void TestFunc_int(int a)
// 	{
// 		xmLogMsg(xm::Log_Default,"TestFunc_int");
// 	}
// 
// 	static void TestFunc_Object(MonoObject* pObj)
// 	{
// 		xmLogMsg(xm::Log_Default,"TestFunc_Object");
// 	}
// 
// 	static void TestFunc_intStringObj(int a,MonoString* b,MonoObject* c)
// 	{
// 		xmLogMsg(xm::Log_Default,"TestFunc_intStringObj");
// 	}
// 
// 
// 	static void ConnectEventImpl(int senderPtr,MonoString* signal,MonoObject* pReciver,MonoString* slot)
// 	{
// 		ScriptSignalKey key;
// 
// 		xm::ScriptDevice* pSD	= xm::GetScriptDevice();
// 		const char* pszSignal	= pSD->GetStringValue(signal);
// 		const char* pszSlot		= pSD->GetStringValue((ScriptObjectHandle)slot);
// 
// 
// 		S3AObject* pSender = (S3AObject*)senderPtr;
// 		key.m_className = NULL != pSender ? pSender->GetClass()->GetName() : "";
// 		key.m_signal = pszSignal;
// 
// 
// 		ScriptEventBinder* pBinder = pSD->CreateScriptEventBinder(key);
// 		if (NULL != pBinder)
// 		{
// 			pBinder->BindScriptEvent(pSender,key.m_signal,pReciver,pszSlot);
// 
// 			ScriptEventAgent* pAgent = (ScriptEventAgent*) pSender->GetData("ScriptAgent");
// 			if (NULL == pAgent)
// 			{
// 				pAgent = new ScriptEventAgent();
// 				pSender->SetData("ScriptAgent",(xmInt32)pAgent);
// 			}
// 			pAgent->AddBinder(pBinder);
// 		}else{
// 			xmLogError(xm::Log_Default,"Fail to bind event");
// 		}
// 
// 
// 	}
// 
// 	static void DisconnectEventImpl(int senderPtr,MonoString* signal,MonoObject* pReciver,MonoString* slot)
// 	{
// 		xm::ScriptDevice* pSD = xm::GetScriptDevice();
// 
// 		S3AObject* pSender = (S3AObject*)senderPtr;
// 		const char* pszSignal	= pSD->GetStringValue((ScriptObjectHandle)signal);
// 		const char* pszSlot		= pSD->GetStringValue((ScriptObjectHandle)slot);
// 
// 		ScriptEventAgent* pAgent = (ScriptEventAgent*) pSender->GetData("ScriptAgent");
// 		ScriptEventBinderKey key = {senderPtr,pszSignal,(ScriptObjectHandle)pReciver,pszSlot};
// 		ScriptEventBinder* pBinder = pAgent->TakeBinder(key);
// 		if (NULL != pBinder)
// 		{
// 			pBinder->UnBindScriptEvent();
// 			pBinder->Release();
// 		}
// 	}

	static MonoObject* MonoGameObject_GetScript(int goPtr,MonoString* pScriptNameM)
	{
		return NULL;
// 		GameObject* pGameObj = (GameObject*)goPtr;
// 		std::string strScriptName = mono_string_to_utf8(pScriptNameM);
// 
// 		ScriptObject* pRetScriptObject = NULL;
// 		for (UINT nCnt = 0; nCnt < pGameObj->GetComponentNumber(); ++nCnt)
// 		{
// 			//Component* pComp = pGameObj->GetComponentByIndex(nCnt);
// 			ScriptComponent* pScriptComp = NULL;//S3ASafeCast<ScriptComponent>(pComp);
// 			if (pScriptComp == NULL)
// 				continue;
// 
// 			pRetScriptObject = pScriptComp->GetScriptObject();
// 			if (pRetScriptObject == NULL)
// 				continue;
// 
// 			if ( stricmp(pRetScriptObject->GetName(),strScriptName.c_str()) == 0)
// 			{
// 				break;
// 			}
// 		}
// 
// 		MonoScriptObject* pMonoScriptObject = (MonoScriptObject*)pRetScriptObject;
// 		return NULL != pMonoScriptObject ? pMonoScriptObject->GetMonoObject() : NULL;
	}

}

void MonoInternalInit()
{	
	//xm::MonoUtil::Init();

	mono_add_internal_call("EngineInternal::MonoGameObject_GetScript(int,string)",ma::MonoGameObject_GetScript);
	//mono_add_internal_call("EngineInternal::ConnectEventImpl(int,string,object,string)",xm::ConnectEventImpl);
	//mono_add_internal_call("EngineInternal::DisonnectEventImpl(int,string,object,string)",xm::DisconnectEventImpl);


	//mono_add_internal_call("EngineInternal::TestFunc_int(int)",xm::TestFunc_int);
	//mono_add_internal_call("EngineInternal::TestFunc_Object(object)",xm::TestFunc_Object);
	//mono_add_internal_call("EngineInternal::TestFunc_intStringObj(int,object,string",xm::TestFunc_intStringObj);
}


