#ifndef  _GAMEOBJ__H__
#define  _GAMEOBJ__H__

#include "SceneNode.h"


namespace ma
{
	class Component;
	class Scene;
	class IPhysicsObject;
	class SceneVisiter;

	class FRAMWORK_API GameObject : public Object
	{
		DECL_OBJECT(GameObject)

	public:
		GameObject(SceneNode* pSceneNode = NULL, const char* pName = NULL);

		~GameObject();

		bool			TravelScene(SceneVisiter* pVisiter);

 		//void			Render();
 
 		//void			Update();

		//void			Awak();
 
 		//void			Start();
 
 		//void			Stop();

		//Scene*			GetScene() {return m_pScene;}
		SceneNode*		GetSceneNode() {return m_pScenNode;}

		GameObject*		Clone(const char* pName);

		//GameObject*		CreateChild(const char* pszName);

		template<class T>
		T*				CreateComponent();

		UINT			GetComponentNumber() {return m_arrComp.size();}

		Component*		GetComponentByIndex(UINT index) {return m_arrComp[index];}
		
		IPhysicsObject*	GetPhyscisObject() {return m_pPhyscisObject;}

		virtual void	Serialize(Serializer& sl, const char* pszLable = "GameObject");
	
	protected:
		void			UpdateAABB();

		void			SetScene(Scene* pScene);

		void			AddComponent(Component* pComponent);

	private:
		IPhysicsObject*				m_pPhyscisObject;

		SceneNode*					m_pScenNode;

		std::vector<Component*>		m_arrComp;
	
	};

	template<class T>
	T*	GameObject::CreateComponent()
	{
		T* pComponent = new T(this);
		AddComponent(pComponent);
		return pComponent;
	}

}



#endif
