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
		if (m_pScriptObject)
		{
			GetScriptSystem()->DeleteScriptObject(m_pScriptObject);
			m_pScriptObject = NULL;
		}
	}

	IScriptObject* ScriptComponent::CreatScriptObject(const char* pszName)
	{
		m_pScriptObject = GetScriptSystem()->CreateScriptObject(pszName);
		return m_pScriptObject;
	}

	void ScriptComponent::Start()
	{
		void* params[1];
		params[0] = &m_pGameObject;
		m_pScriptObject->InvokeMethod("SetGameObjPtr",1,params);

		m_pScriptObject->InvokeMethod("Start");

		return;
	}
 
	void ScriptComponent::Stop()
	{
		if (m_pScriptObject == NULL)
			return;

		m_pScriptObject->InvokeMethod("Stop");

	}
 
	void ScriptComponent::Update()
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
			m_pScriptObject = GetScriptSystem()->CreateScriptObject(strScriptName.c_str()/*,m_pGameObject*/);
		}
		else
		{
			std::string strScriptName = m_pScriptObject->GetName();
			sl.Serialize(strScriptName);	
		}

		m_pScriptObject->Serialize(sl);

		sl.EndSection();
	}


}


