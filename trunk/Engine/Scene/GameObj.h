#ifndef  _GAMEOBJ__H__
#define  _GAMEOBJ__H__

#include "Engine/Scene/SceneNode.h"


namespace ma
{
	class Component;
	class Scene;
	class IPhysicsObject;

	class ENGINE_API GameObject : public SceneNode
	{
		DECL_OBJECT(GameObject)

	public:
		GameObject(Scene* pScene = NULL, const char* pName = NULL);

		~GameObject();

		void			Render();

		void			Update();

		void			ParalleUpdate(float fTimeElapsed);

		void			FixedUpdate(float fTimeElapsed);

		void			LateUpdate(float fTimeElapsed);

		void			Start();

		void			Stop();

		void			AddComponent(Component* pComponent);
		
		IPhysicsObject*	GetPhyscisObject() {return m_pPhyscisObject;}

		void			SetPhyscisObject(IPhysicsObject* pPhyscisObject) {m_pPhyscisObject = pPhyscisObject;} 

		void			SyncToPhysics();

		void			SyncFromPhysics();

		virtual void	Serialize(SerializeListener& sl, const char* pszLable = "GameObject");
	
	protected:
		virtual void UpdateAABB();

	private:
		IPhysicsObject* m_pPhyscisObject;

		std::vector<Component*> m_arrComp;
	
	};
}



#endif
