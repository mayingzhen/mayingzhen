#include "GameObj.h"
#include "Component/Component.h"

namespace ma
{
	IMPL_OBJECT(GameObject,Object)
	
	GameObject::GameObject(SceneNode* pSceneNode, const char* pName)
	{
		m_pScenNode = pSceneNode;
		Scene* pScene = m_pScenNode ? m_pScenNode->GetSene() : NULL;
		IPhysicsScene* pPhyScene = pScene ? pScene->GetPhysicsScene() : NULL;
		m_pPhyscisObject = pPhyScene ? pPhyScene->CreatePhysicsObject(this) : NULL;
	}

	GameObject::~GameObject()
	{
		//SAFE_DELETE(m_pPhyscisObject);
	}

	bool GameObject::TravelScene(SceneVisiter* pVisiter)
	{
		bool bTraveling = pVisiter->VisiteGameObjectBegin(this);

		if (bTraveling)
		{
			for (UINT nCnt = 0; nCnt < m_arrComp.size(); ++nCnt)
			{
				bTraveling = pVisiter->VisiteComponent(m_arrComp[nCnt]);
				if (!bTraveling)
				{
					break;
				}
			}
		}

		pVisiter->VisiteGameObjectEnd(this);

		return bTraveling;
	}

// 	void GameObject::SetScene(Scene* pScene)
// 	{
// 		ASSERT(pScene);
// 		ASSERT(m_pScene == NULL);
// 		m_pScene = pScene;
// 		IPhysicsScene* pPhyScene = pScene ? pScene->GetPhysicsScene() : NULL;
// 		if (pPhyScene)
// 		{
// 			m_pPhyscisObject = pPhyScene->CreatePhysicsObject(this);
// 		}
// 	}

// 	void GameObject::Render()
// 	{
// 		Uint nChild = m_arrChildNode.size();
// 		for (UINT i = 0; i < nChild; ++i)
// 		{
// 			GameObject* pChild = static_cast<GameObject*>(m_arrChildNode[i]);
// 			ASSERT(pChild);
// 			if (pChild == NULL)
// 				continue;
// 
// 			pChild->Render();
// 		}
// 		
// 		for (UINT i = 0; i < m_arrComp.size(); ++i)
// 		{
// 			m_arrComp[i]->Render();
// 		}
// 	}

// 	void GameObject::Update()
// 	{
// 		Uint nChild = m_arrChildNode.size();
// 		for (UINT i = 0; i < nChild; ++i)
// 		{
// 			GameObject* pChild = static_cast<GameObject*>(m_arrChildNode[i]);
// 			ASSERT(pChild);
// 			if (pChild == NULL)
// 				continue;
// 
// 			pChild->Update();
// 		}
// 
// 		for (UINT i = 0; i < m_arrComp.size(); ++i)
// 		{
// 			m_arrComp[i]->Update();
// 		}
// 	}

// 	void GameObject::Awak()
// 	{
// 		for(UINT i = 0; i < m_arrChildNode.size(); ++i)
// 		{
// 			GameObject* pGameObj = (GameObject*)m_arrChildNode[i];
// 			pGameObj->SetParentSceneNode(this);
// 			pGameObj->SetScene(m_pScene);
// 		}
// 
// 		for(UINT i = 0; i < m_arrComp.size(); ++i)
// 		{
// 			m_arrComp[i]->SetGameObject(this);
// 		}
// 	}

// 	void GameObject::Start()
// 	{
// 		Uint nChild = m_arrChildNode.size();
// 		for (UINT i = 0; i < nChild; ++i)
// 		{
// 			GameObject* pChild = static_cast<GameObject*>(m_arrChildNode[i]);
// 			ASSERT(pChild);
// 			if (pChild == NULL)
// 				continue;
// 
// 			pChild->Start();
// 		}
// 
// 		for (UINT i = 0; i < m_arrComp.size(); ++i)
// 		{
// 			m_arrComp[i]->Start();
// 		}
// 	}

// 	void GameObject::Stop()
// 	{
// 		Uint nChild = m_arrChildNode.size();
// 		for (UINT i = 0; i < nChild; ++i)
// 		{
// 			GameObject* pChild = static_cast<GameObject*>(m_arrChildNode[i]);
// 			ASSERT(pChild);
// 			if (pChild == NULL)
// 				continue;
// 
// 			pChild->Stop();
// 		}
// 
// 		for (UINT i = 0; i < m_arrComp.size(); ++i)
// 		{
// 			m_arrComp[i]->Stop();
// 		}
// 	}

	void GameObject::AddComponent(Component* pComponent)
	{
		if (pComponent == NULL)
			return;

		std::vector<Component*>::iterator it = find(m_arrComp.begin(),m_arrComp.end(),pComponent);
		if (it != m_arrComp.end())
			return;

		m_arrComp.push_back(pComponent);
	}

// 	GameObject*	GameObject::CreateChild(const char* pszName)
// 	{
// 		GameObject* pChild = new GameObject(m_pScene, pszName);
// 		//AddChildNode(pChild);
// 		return pChild;
// 	}

// 	void GameObject::SyncToPhysics()
// 	{
// 		if (m_pPhyscisObject == NULL)
// 			return;
// 	
// 		//if ( m_pPhyscisObject->IsKinematic() )
// 			m_pPhyscisObject->SetTransformWS( m_pScenNode->GetTransform(TS_WORLD) );
// 	}

// 	void GameObject::SyncFromPhysics()
// 	{
// 		if (m_pPhyscisObject == NULL)
// 			return;
// 
// 		//if ( !m_pPhyscisObject->IsKinematic() )
// 			m_pScenNode->SetTransform( m_pPhyscisObject->GetTransformWS(), TS_WORLD );
// 	}

	void GameObject::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sName,"m_sName");

		if (sl.IsReading())
		{
			UINT nSize = 0;

			sl.Serialize(nSize,"arrCompSize");

			for (UINT nCnt = 0; nCnt < nSize; ++nCnt)
			{
				std::string strClassName;
				sl.Serialize(strClassName,"classType");
			
				ObjectFactoryManager& objFac = ObjectFactoryManager::GetInstance();
				Component* pComp = static_cast<Component*>(objFac.CreateObjectArg(strClassName.c_str(),this));
				m_arrComp.push_back(pComp);

				pComp->Serialize(sl,strClassName.c_str());
			}
		}
		else
		{
			UINT nSize = m_arrComp.size();

			sl.Serialize(nSize,"arrCompSize");

			for (UINT nCnt = 0; nCnt < nSize; ++nCnt)
			{

				const RTTIClass* pClass = m_arrComp[nCnt]->GetClass();
				ASSERT(pClass);
				if (pClass == NULL)
					continue;

				std::string strClassName = pClass->GetName();
				sl.Serialize(strClassName,"classType");

				m_arrComp[nCnt]->Serialize(sl,strClassName.c_str());

			}
		}

		sl.EndSection();
	}

	GameObject* GameObject::Clone(const char* pName)
	{
		ASSERT(m_pScenNode);
		SceneNode* pClonNode = m_pScenNode->Clone(pName);
		GameObject* pClonGameObj = pClonNode->GetGameObject();
		pClonGameObj->SetName(pName);

		return pClonGameObj;
	}

	void GameObject::UpdateAABB()
	{

	}

}


