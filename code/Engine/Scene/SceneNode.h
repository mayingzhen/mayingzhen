#ifndef  _SceneNode__H__
#define  _SceneNode__H__

#include "NodeTransform.h"
#include "Component.h"

namespace ma
{
	class Component;
	class Scene;
	class IPhysicsObject;
	class SceneVisiter;
	class CullNode;
	class CullTree;

	class ENGINE_API SceneNode : public NodeTransform
	{
		DECL_OBJECT(SceneNode)

	public:
		SceneNode(Scene* pScene,const char* pName = NULL);

		~SceneNode();

		void				Update();

		// Component
		template<class T>
		T*					CreateComponent();
		UINT				GetComponentNumber() {return m_arrComp.size();}
		Component*			GetComponentByIndex(UINT index) {return m_arrComp[index].get();}
		template<class T>
		void				GetTypeComponent(std::vector<T*>& arrTyeComp);
		template<class T>
		T*					GetTypeComponent();
		void				AddComponent(Component* pComponent);
		void				RemoveComponent(Component* pComponent);
						

		// Child Parent
		SceneNode*			GetParent() {return m_pParent;}
		void				AddChild(SceneNode* pChild);
		void				RemoveChild(SceneNode* pChild);
		void				RemoveAllChild();	
		SceneNode*			FindChildNode(const char* pszName);	

		Scene*				GetScene() {return m_pScene;}

		SceneNode*			Clone(const char* pName);

		void				Serialize(Serializer& sl, const char* pszLable = "GameObject");

		void				OnTransformChange();

		void				SetUserData(const char* pszKey,void* pData);
		void*				GetUserData(const char* pszKey);

		void				SetLastVisibleFrame(UINT nFrame) {m_nLastVisibleFrame = nFrame;}
		UINT				GetLastVisibleFrame() {return m_nLastVisibleFrame;}

	private:
		void				SerializeChild(Serializer& sl,const char* pszLable);
		void				SerializeComp(Serializer& sl,const char* pszLable);

		void				SetParent(SceneNode* pParent);

	private:
		std::vector<ComponentPtr>	m_arrComp;
		SceneNode*					m_pParent;
		Scene*						m_pScene;
		typedef std::vector< RefPtr<SceneNode> > VEC_CHILD;
		VEC_CHILD					m_arrChild;

		UINT						m_nLastVisibleFrame;

		typedef std::map<std::string, void*> MAP_USER_DATA;
		MAP_USER_DATA				m_mapUserData;
	};

	DeclareRefPtr(SceneNode);


	template<class T>
	inline T* SceneNode::CreateComponent()
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
	inline void SceneNode::GetTypeComponent(std::vector<T*>& arrTyeComp)
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

	template<class T>
	T*	SceneNode::GetTypeComponent()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			T* pComponent = SafeCast<T>( m_arrComp[i].get() );
			if (pComponent != NULL)
			{
				return pComponent;
			}
		}
		return NULL;
	}
}



#endif
