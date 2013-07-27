#include "ScriptComponent.h"

namespace ma
{

	ScriptComponent::ScriptComponent(GameObject* pGameObj)
	:Component(pGameObj)
	{
		m_pScriptObject = NULL;
	}

	ScriptComponent::~ScriptComponent()
	{
	}

	void ScriptComponent::Start()
	{
 		IScriptDevice* pScriptDevice = GetScriptDevice();
 		ASSERT(pScriptDevice);
 		if (pScriptDevice == NULL)
 			return ;

		void* params[1];
 		params[0] = &m_pGameObject;
		m_pScriptObject->InvokeMethod("SetGameObject",1,params);

		m_pScriptObject->InvokeMethod("Start");

		return;
	}

	void ScriptComponent::Stop()
	{
		if (m_pScriptObject == NULL)
			return;

		m_pScriptObject->InvokeMethod("Stop");
		
		IScriptDevice* pScriptDevice = GetScriptDevice();
		ASSERT(pScriptDevice);
		if (pScriptDevice == NULL)
			return;

		pScriptDevice->DestoryScriptObject(m_pScriptObject);
		m_pScriptObject = NULL;
	}

	void ScriptComponent::Update(float fTimeElapsed)
	{
		if (m_pScriptObject == NULL)
			return;
		
		m_pScriptObject->InvokeMethod("Update");
	}

	void ScriptComponent::OnCollisionEnter(const Collision& collData)
	{
		const Collision* ptr = &collData;
		void* params[1];
		params[0] = &ptr;
		m_pScriptObject->InvokeMethod("OnCollisionEnter",1,params);
	}

	void ScriptComponent::OnCollisionExit(const Collision& collData)
	{
		const Collision* ptr = &collData;
		void* params[1];
		params[0] = &ptr;
 		m_pScriptObject->InvokeMethod("OnCollisionExit",1,params);
	}

	void ScriptComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		if (sl.IsReading())
		{
			std::string strScriptName;
			sl.Serialize(strScriptName);
			m_pScriptObject = GetScriptDevice()->CreateScriptObject(strScriptName.c_str());
		}
		else
		{
			std::string strScriptName = m_pScriptObject->GetName();
			sl.Serialize(strScriptName);	
		}

		m_pScriptObject->Serialize(sl);

		sl.EndSection();
	}

	IScriptObject* ScriptComponent::CreatScriptObject(const char* pszName)
	{
		m_pScriptObject = GetScriptDevice()->CreateScriptObject(pszName);
		return m_pScriptObject;
	}
}


