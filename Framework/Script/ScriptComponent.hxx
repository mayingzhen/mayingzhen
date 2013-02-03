#include <Framework/Script/ScriptComponent.h>
#include <Framework/Script/IScriptDevice.h>
#include <Framework/Script/EdScriptObject.h>
#include <Framework/Script/IScriptObject.h>

namespace ma
{

	ScriptComponent::ScriptComponent()
	{
		m_pScriptObject = NULL;
		m_pEdScriptObject = NULL;
		m_pActiveScriptObject = NULL;

		// Method
		m_pSetObject = NULL;
		m_pStart = NULL;
		m_pUpdate = NULL;
		m_pLateUpdate = NULL;
		m_pFixedUpdate = NULL;
		m_pStop = NULL;
		m_pOnContactNotify = NULL;
	}

	ScriptComponent::~ScriptComponent()
	{
	}

	void ScriptComponent::SetPublicFieldValue(ScriptObject* pScriptObject, TiXmlElement* pEleClass)
	{
		if (pScriptObject == NULL || pEleClass == NULL)
			return;

		TiXmlElement* pEleField = pEleClass->FirstChildElement("EdField");
		while (pEleField)
		{
			const char* pFieldName = pEleField->Attribute("name");
			const char* pValue = pEleField->Attribute("value");
			const char* pTypeName = pEleField->Attribute("sTypeName");
			assert(pTypeName);
			if (pTypeName == NULL)
				continue;

			if ( strcmp(pTypeName,"int") == 0 )
			{
				pScriptObject->SetInt(pFieldName, atoi(pValue) );
			}
			else if (strcmp(pTypeName,"uint") == 0)
			{
				pScriptObject->SetUint(pFieldName, (xmUint)atoi(pValue) );
			}
			else if (strcmp(pTypeName,"float") == 0)
			{
				pScriptObject->SetFloat(pFieldName, (float)atof(pValue) );
			}
			else if ( strcmp(pTypeName,"D3DXVECTOR3") == 0)
			{
				TiXmlElement* pEleVecor3 = pEleField->FirstChildElement("D3DXVECTOR3");
				//D3DXVECTOR3 vector3 = ParseD3DXVECTOR3(pEleVecor3);
				//pScriptObject->SetVector3(pFieldName,vector3);
			}
		
			pEleField = pEleField->NextSiblingElement("EdField");
		}
	}

	void ScriptComponent::CacheMethod()
	{
		m_pSetObject = m_pScriptObject->GetScriptMethodHandle("SetGameObject",1);
		m_pStart = m_pScriptObject->GetScriptMethodHandle("Start");
		m_pStop = m_pScriptObject->GetScriptMethodHandle("Stop");
		m_pUpdate = m_pScriptObject->GetScriptMethodHandle("Update");
		m_pLateUpdate = m_pScriptObject->GetScriptMethodHandle("LateUpdate");
		m_pFixedUpdate = m_pScriptObject->GetScriptMethodHandle("FixedUpdate",1);
		m_pOnContactNotify = m_pScriptObject->GetScriptMethodHandle("OnContactNotify",1);
	}

	void ScriptComponent::Start()
	{
 		ScriptDevice* pScriptDevice = GetScriptDevice();
 		assert(pScriptDevice);
 		if (pScriptDevice == NULL)
 			return ;
 
 		const char* pszClassName = m_pEdScriptObject->GetName();
		m_pScriptObject = pScriptDevice->CreateScriptObject(pszClassName);
		
		SetPublicFieldValue(m_pScriptObject,m_pEdScriptObject->GetTiXmlElement());

		CacheMethod();

		void* params[1];
 		params[0] = &m_pGameObject;
		m_pScriptObject->InvokeMethod(m_pSetObject,1,params);

		m_pScriptObject->InvokeMethod(m_pStart);

		m_pActiveScriptObject = m_pScriptObject;

		return;
	}

	void ScriptComponent::Stop()
	{
		m_pActiveScriptObject = m_pEdScriptObject;

		if (m_pScriptObject == NULL)
			return;

		m_pScriptObject->InvokeMethod(m_pStop);
		
		ScriptDevice* pScriptDevice = GetScriptDevice();
		assert(pScriptDevice);
		if (pScriptDevice == NULL)
			return;

		pScriptDevice->DestoryScriptObject(m_pScriptObject);
		m_pScriptObject = NULL;
	}

	void ScriptComponent::Update(float fTimeElapsed)
	{
		if (m_pScriptObject == NULL)
			return;
		
		m_pScriptObject->InvokeMethod(m_pUpdate);
	}

	void ScriptComponent::LateUpdate(float fTimeElapsed)
	{
		if (m_pScriptObject == NULL)
			return;

		m_pScriptObject->InvokeMethod(m_pLateUpdate);
	}

	void ScriptComponent::FixedUpdate(float fTimeElapsed)
	{
		if (m_pScriptObject == NULL)
			return;

		void* params[1];
		params[0] = (void*)&fTimeElapsed;
		m_pScriptObject->InvokeMethod(m_pFixedUpdate,1,params);
	}

	void ScriptComponent::OnContactNotify(GameObject* pGameObj)
	{
		if (m_pScriptObject == NULL)
			return;

		void* params[1];
		params[0] = &pGameObj;
		m_pScriptObject->InvokeMethod(m_pOnContactNotify,1,params);
	}

	void ScriptComponent::SetScriptByName(const char* pszName)
	{
		if (pszName == NULL)
			return;

		ScriptDevice* pScriptDevice = GetScriptDevice();
		if (pScriptDevice == NULL)
			return;

		TiXmlElement* pXMlClass = pScriptDevice->GetXmlClassByName(pszName);
		assert(pXMlClass);
		if (pXMlClass == NULL)
			return;

		xmSafeDelete(m_pEdScriptObject);
		m_pEdScriptObject = new EdScriptObject(pXMlClass);
		m_pActiveScriptObject = m_pEdScriptObject;

		assert(m_pScriptObject == NULL);
		if (m_pScriptObject)
		{
			pScriptDevice->DestoryScriptObject(m_pScriptObject);
		}
	}
}


