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
	class CullTree;

	DeclareRefPtr(GameObject);

	class ENGINE_API GameObject : public Object
	{
		DECL_OBJECT(GameObject)

	public:
		class  CCallback
		{
		public:
			CCallback(){}
			virtual ~CCallback(){}
			virtual void OnNodeProcessed(const GameObject* pNode){}
			virtual void OnNodeBeginShow(const GameObject* pNode){}
			virtual void OnNodeEndShow(const GameObject* pNode){}
			virtual void OnNodeDestroyed(const GameObject* pNode){}
			virtual void OnNodeAttached(const GameObject* pNode){}
			virtual void OnNodeDetached(const GameObject* pNode){}
			virtual void OnNodeEndMatrix(const GameObject* pNode){}
			virtual void OnNodeLoadOver(GameObject* pNode){}
		};

	public:
		GameObject(const char* pName = NULL);

		~GameObject();

		void				Update();

		void				BeginShow(Camera* pCamera);

		void				Show(Camera* pCamera, bool bCascade);

		void				EndShow(Camera* pCamera);

		bool				TravelScene(SceneVisiter* pVisiter);

		SceneNode*			GetSceneNode() {return m_pScenNode;}

		GameObjectPtr		Clone(const char* pName);

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

		void				SetCullTree(CullTree* pCullTree);

		CullTree*			GetCullTree() const{return m_pCullTree;}

		// 
		GameObject*			GetParent() {return m_pParent;}

		void				AddChild(GameObjectPtr pChild);

		void				RemoveChild(GameObjectPtr pChild);

		void				RemoveAllChild();	

	protected:
		void				UpdateAABB();

		void				SetScene(Scene* pScene);

		void				AddComponent(Component* pComponent);

		void				SetParent(GameObject* pParent);

	private:
		void				SerializeChild(Serializer& sl, std::vector<GameObjectPtr>& arrChild,
			GameObject* pParent,const char* pszLable);


// 		enum CHANGE_TYPE
// 		{
// 			CT_NONE = 0x00,
// 			CT_PART = 0x01,
// 			CT_FROMPARENT = 0x02,
// 			CT_NOTIFY = 0x04,
// 		};
// 		void				SetNeedChange(CHANGE_TYPE eChangeType);
//
// 		enum AABB_CHANGE_TYPE
// 		{
// 			ACT_NONE = 0x00,
// 			ACT_SELF_MATRIX = 0x01,
// 			ACT_SELF_CUSTOM = 0x02,//自定义包围盒
// 			ACT_NOTIFY = 0x04,
// 		};
// 		
// 		void				UpdateWorldMatrix();
// 
// 		void				UpdateWorldBoundingBox();


	private:
		IPhysicsObject*				m_pPhyscisObject;

		SceneNode*					m_pScenNode;

		std::vector<Component*>		m_arrComp;

		GameObject*					m_pParent;

		std::vector<GameObjectPtr>	m_arrChild;

		CullNode*					m_pCullNode;
		
		CullTree*					m_pCullTree;

		CCallback*					m_pCallback;

//		int							m_nNeedChange;
//
// 		int							m_nAABBChangeType;	
// 
// 		AABB						m_AABB;
// 
// 		AABB						m_worldAABB;
// 
// 		Matrix4x4					m_matWorld;
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


}



#endif
