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

		const AABB&			GetAABBWS();

		// Component
		template<class T>
		ref_ptr<T>			CreateComponent();

		UINT				GetComponentNumber() {return m_arrComp.size();}

		ref_ptr<Component>	GetComponentByIndex(UINT index) {return m_arrComp[index];}

		template<class T>
		void				GetTypeComponent(std::vector<T*>& arrTyeComp);
	
		// physic
		IPhysicsObject*		GetPhyscisObject() {return m_pPhyscisObject;}

		// CullNode
		void				SetCullNode(CullNode* pCullNode) {m_pCullNode = pCullNode;}

		CullNode*			GetCullNode() {return m_pCullNode;}

		void				SetCullTree(CullTree* pCullTree);

		CullTree*			GetCullTree() const{return m_pCullTree;}

		// Child Parent
		GameObject*			GetParent() {return m_pParent;}

		void				AddChild(GameObjectPtr pChild);

		void				RemoveChild(GameObjectPtr pChild);

		void				RemoveAllChild();	

		GameObjectPtr		Clone(const char* pName);

		void				Serialize(Serializer& sl, const char* pszLable = "GameObject");

	protected:
		void				UpdateAABB();

		void				SetScene(Scene* pScene);

		void				AddComponent(ComponentPtr pComponent);

		void				SetParent(GameObject* pParent);

	private:
		void				SerializeChild(Serializer& sl,const char* pszLable);

		void				SerializeComp(Serializer& sl,const char* pszLable);

	private:
		IPhysicsObject*				m_pPhyscisObject;

		SceneNode*					m_pScenNode;

		std::vector<ComponentPtr>	m_arrComp;

		GameObject*					m_pParent;

		std::vector<GameObjectPtr>	m_arrChild;

		CullNode*					m_pCullNode;
		
		CullTree*					m_pCullTree;

		CCallback*					m_pCallback;

		AABB						m_AABB;

		AABB						m_worldAABB;
	};


	template<class T>
	inline ref_ptr<T>	GameObject::CreateComponent()
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
	inline void GameObject::GetTypeComponent(std::vector<T*>& arrTyeComp)
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			T* pComponent = SafeCast<T>( m_arrComp[i].get() );
			if (pComponent != NULL)
			{
				arrTyeComp.push_back(pComponent);
			}
		}
	}
}



#endif
