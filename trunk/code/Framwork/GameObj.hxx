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


	void GameObject::AddComponent(Component* pComponent)
	{
		if (pComponent == NULL)
			return;

		std::vector<Component*>::iterator it = find(m_arrComp.begin(),m_arrComp.end(),pComponent);
		if (it != m_arrComp.end())
			return;

		m_arrComp.push_back(pComponent);
	}

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

	ICharaControll* GameObject::GetCharController()
	{
		return NULL;
	}

	void GameObject::AddCollisionListener(GameObject* pGameObjB)
	{
		ICharaControll* pCharcterB = pGameObjB ? pGameObjB->GetCharController() : NULL;
		IPhysicsObject* m_pPhyscisObjectB = pGameObjB ? pGameObjB->GetPhyscisObject() : NULL;

		if (m_pPhyscisObject == NULL)
		{
			ICharaControll* pCharcter = GetCharController();
			ASSERT(pCharcter);
			if (pCharcter == NULL)
				return;

			//if (pCharcterB)
			//	pCharcter->AddCollisionListener(pCharcterB);
			//else
			//	pCharcter->AddCollisionListener(pPhysicsActorB);
		}
		else
		{
			//if (pCharcterB)
			//	m_pPhyscisObject->AddCollisionListener(pCharcterB);
			//else
			//	m_pPhyscisObject->AddCollisionListener(m_pPhyscisObjectB);
		}
	}

	void GameObject::RemoveCollisionListener(GameObject* pGameObjB)
	{

	}

}


