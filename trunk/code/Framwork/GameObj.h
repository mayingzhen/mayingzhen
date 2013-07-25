#ifndef  _GameObject__H__
#define  _GameObject__H__

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

		bool				TravelScene(SceneVisiter* pVisiter);

		SceneNode*			GetSceneNode() {return m_pScenNode;}

		GameObject*			Clone(const char* pName);

		template<class T>
		T*					CreateComponent();

		UINT				GetComponentNumber() {return m_arrComp.size();}

		Component*			GetComponentByIndex(UINT index) {return m_arrComp[index];}

		template<class T>
		T*					GetTypeComponentFirst();

		template<class T>
		T*					GetTypeComponentNumber();

		template<class T>
		T*					GetTypeComponentByIndex(UINT index);

		void				Serialize(Serializer& sl, const char* pszLable = "GameObject");

		// physic
		IPhysicsObject*		GetPhyscisObject() {return m_pPhyscisObject;}
		
		ICharaControll*		GetCharController();

		void				AddCollisionListener(GameObject* pGameObjB);

		void				RemoveCollisionListener(GameObject* pGameObjB);

	protected:
		void				UpdateAABB();

		void				SetScene(Scene* pScene);

		void				AddComponent(Component* pComponent);

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

	template<class T>
	T*	GameObject::GetTypeComponentFirst()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			if (T::StaticGetClass() == m_arrComp[i].GetClass())
			{
				return m_arrComp[i];
			}
		}
		return NULL;
	}

	template<class T>
	T*	GameObject::GetTypeComponentNumber()
	{
		UINT nNumber = 0;
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			if (T::StaticGetClass() == m_arrComp[i].GetClass())
			{
				++nNumber;
			}
		}
		return nNumber;
	}

	template<class T>
	T*	GameObject::GetTypeComponentByIndex(UINT index)
	{
		UINT nNumber = 0;
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			if (T::StaticGetClass() == m_arrComp[i].GetClass() && nNumber == index)
			{
				return m_arrComp[i];
			}
		}
		return NULL;
	}

}



#endif
