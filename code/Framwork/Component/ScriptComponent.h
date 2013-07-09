#ifndef __ScriptComponent_H__
#define __ScriptComponent_H__

#include "Component.h"

namespace ma
{
	class ScriptObject;
	class EdScriptObject;

	class FRAMWORK_API ScriptComponent : public Component
	{
	public:

		ScriptComponent(GameObject* pGameObj);

		~ScriptComponent();

		virtual void Start();

		virtual void Stop();

		virtual void Update(float fTimeElapsed);

		virtual void LateUpdate(float fTimeElapsed);

		virtual void FixedUpdate(float fTimeElapsed);

		virtual void OnContactNotify(GameObject* pGameObj);


		ScriptObject* GetScriptObject() {return m_pScriptObject;}

		void		  SetScriptByName(const char* pszName);

		ScriptObject* GetActiveScriptObject() {return m_pActiveScriptObject;}	

	private:
		// 将XMl内保存的变量值设置到ScriptObject上
		void SetPublicFieldValue(ScriptObject* pScriptObject, TiXmlElement* pEleClass);

		void CacheMethod();

	private:
		//HScriptResource m_hScriptRes;

		ScriptObject* m_pScriptObject;

		EdScriptObject* m_pEdScriptObject;

		ScriptObject*	m_pActiveScriptObject;

		// Method
		ScriptMethodHandle m_pSetObject;
		ScriptMethodHandle m_pStart;
		ScriptMethodHandle m_pUpdate;
		ScriptMethodHandle m_pLateUpdate;
		ScriptMethodHandle m_pFixedUpdate;
		ScriptMethodHandle m_pStop;
		ScriptMethodHandle m_pOnContactNotify;
	};
}


#endif //__ScriptComponent_H__
