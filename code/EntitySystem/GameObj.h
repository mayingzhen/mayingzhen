#ifndef  _GameObject__H__
#define  _GameObject__H__

#include "SceneNode.h"
#include "Component/Component.h"

namespace ma
{
	class Component;
	class Scene;
	class IPhysicsObject;
	class SceneVisiter;


	class ENTITYSYSTEM_API GameObject : public Object
	{
		DECL_OBJECT(GameObject)

	public:
		GameObject(const char* pName = NULL);

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
		UINT				GetTypeComponentNumber();

		template<class T>
		T*					GetTypeComponentByIndex(UINT index);

		void				Serialize(Serializer& sl, const char* pszLable = "GameObject");

		// physic
		IPhysicsObject*		GetPhyscisObject() {return m_pPhyscisObject;}

		void				AddCollisionListener(GameObject* pGameObjB,CollisionListener* pCallBack);

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
	inline T*	GameObject::CreateComponent()
	{
		T* pComponent = new T(this);
		AddComponent(pComponent);
		return pComponent;
	}

	template<class T>
	inline T*	GameObject::GetTypeComponentFirst()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			if (T::StaticGetClass() == m_arrComp[i]->GetClass())
			{
				return SafeCast<T>(m_arrComp[i]);
			}
		}
		return NULL;
	}

	template<class T>
	inline UINT	GameObject::GetTypeComponentNumber()
	{
		UINT nNumber = 0;
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			if (T::StaticGetClass() == m_arrComp[i]->GetClass())
			{
				++nNumber;
			}
		}
		return nNumber;
	}

	template<class T>
	inline T*	GameObject::GetTypeComponentByIndex(UINT index)
	{
		UINT nNumber = 0;
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			if (T::StaticGetClass() == m_arrComp[i]->GetClass())
			{
				if (nNumber == index)
				{
					return SafeCast<T>(m_arrComp[i]);
				}

				nNumber++;
			}
		}
		return NULL;
	}

	struct ENTITYSYSTEM_API Collision
	{
		GameObject* m_pGameEntity;
		Vector3	m_vContactsPointWS;
		Vector3	m_vContactsNormalWS;

		Collision()
		{
			m_pGameEntity = NULL;
			m_vContactsPointWS = Vector3(0,0,0);
			m_vContactsNormalWS = Vector3(0,0,0);
		}
	};


}



#endif
