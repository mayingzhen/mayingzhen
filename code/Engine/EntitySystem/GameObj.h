#ifndef  _GameObject__H__
#define  _GameObject__H__

#include "SceneNode.h"
#include "Component.h"

namespace ma
{
	class Component;
	class Scene;
	class IPhysicsObject;
	class SceneVisiter;
	class CullNode;


	class ENGINE_API GameObject : public Object
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

		void				SetCullNode(CullNode* pCullNode) {m_pCullNode = pCullNode;}

		CullNode*			GetCullNode() {return m_pCullNode;}

	protected:
		void				UpdateAABB();

		void				SetScene(Scene* pScene);

		void				AddComponent(Component* pComponent);

	private:
		IPhysicsObject*				m_pPhyscisObject;

		SceneNode*					m_pScenNode;

		std::vector<Component*>		m_arrComp;

		CullNode*					m_pCullNode;
	};

// 	template<class T>
// 	inline T*	GameObject::CreateComponent()
// 	{
// 		T* pComponent = new T(this);
// 		AddComponent(pComponent);
// 		return pComponent;
// 	}

	template<class T>
	inline T*	GameObject::CreateComponent()
	{
		const RTTIClass* pClass = T::StaticGetClass();
		ASSERT(pClass);
		if (pClass == NULL)
			return NULL;

		Object* pObject = ObjectFactoryManager::GetInstance().CreateObjectArg(pClass->GetName(), this);
		ASSERT(pObject);
		if (pObject == NULL)
			return NULL;

		T* pComponent = SafeCast<T>(pObject);
		ASSERT(pComponent);
		if (pComponent == NULL)
			return NULL;

		AddComponent(pComponent);
		return pComponent;
	}


	template<class T>
	inline T*	GameObject::GetTypeComponentFirst()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			if ( m_arrComp[i]->GetClass()->IsA( T::StaticGetClass() ) )
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
			if ( m_arrComp[i]->GetClass()->IsA( T::StaticGetClass() ) )
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
			if ( m_arrComp[i]->GetClass()->IsA( T::StaticGetClass() ) )
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

	struct ENGINE_API Collision
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
