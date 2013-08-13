#ifndef __ScriptComponent_H__
#define __ScriptComponent_H__

#include "Component.h"

namespace ma
{
	class IScriptObject;

	class ENTITYSYSTEM_API ScriptComponent : public Component
	{
	public:

		ScriptComponent(GameObject* pGameObj);

		~ScriptComponent();

		virtual void	Start();

		virtual void	Stop();

		virtual void	Update(float fTimeElapsed);

		virtual void	OnCollisionEnter(const Collision& collData);

		virtual void	OnCollisionExit(const Collision& collData);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "ScriptComponent");

		IScriptObject*	GetScriptObject() {return m_pScriptObject;}

		IScriptObject*	CreatScriptObject(const char* pszName);

	private:

 		IScriptObject*	m_pScriptObject;
	};
}


#endif //__ScriptComponent_H__
