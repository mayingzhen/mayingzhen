#include "GameObj.h"
#include "Component/Component.h"

namespace ma
{
	IMPL_OBJECT(GameObject,Object)
	
	GameObject::GameObject(const char* pName)
	{
		m_sName = pName;
		m_pScenNode = new SceneNode(this);
		m_pPhyscisObject = GetPhysicsSystem() ? GetPhysicsSystem()->CreatePhysicsObject(this) : NULL;
	}

	GameObject::~GameObject()
	{
		SceneNode* pParent = m_pScenNode->GetParent();
		if (pParent)
		{
			pParent->RemoveChildNode(m_pScenNode);
		}
		SAFE_DELETE(m_pScenNode);
		
		if (GetPhysicsSystem() && m_pPhyscisObject)
		{
			GetPhysicsSystem()->DeletePhysicsObject(m_pPhyscisObject);
			m_pPhyscisObject = NULL;
		}

		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			SAFE_DELETE(m_arrComp[i]);
		}
		m_arrComp.clear();
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

		m_pScenNode->Serialize(sl);

		sl.EndSection();
	}

	GameObject* GameObject::Clone(const char* pName)
	{
// 		SceneNode* pParent = m_pScenNode->GetParent();
// 		ASSERT(pParent);
// 		if (pParent == NULL)
// 			return NULL;

		XMLOutputArchive xmlout;
		this->Serialize(xmlout);

		XMLInputArchive xmlin;
		xmlin.Open(xmlout);
		//SceneNode* pClone = pParent->CreateChildNode(pName);
		GameObject* pClone = GetEntitySystem()->CreateGameObject(pName);
		pClone->Serialize(xmlin);

		return pClone;

// 		ASSERT(m_pScenNode);
// 		SceneNode* pClonNode = m_pScenNode->Clone(pName);
// 		GameObject* pClonGameObj = pClonNode->GetGameObject();
// 		pClonGameObj->SetName(pName);
// 
// 		return pClonGameObj;
	}

	void GameObject::UpdateAABB()
	{

	}

// 	ICharaControll* GameObject::GetCharController()
// 	{
// 		return NULL;
// 	}

	void GameObject::AddCollisionListener(GameObject* pGameObjB,CollisionListener* pCallBack)
	{
		ASSERT(m_pPhyscisObject);
		if (m_pPhyscisObject == NULL)
			return;

		IPhysicsObject* pPhyscisObjectB = pGameObjB ? pGameObjB->GetPhyscisObject() : NULL;
		m_pPhyscisObject->AddCollisionListener(pPhyscisObjectB,pCallBack);
	}

	void GameObject::RemoveCollisionListener(GameObject* pGameObjB)
	{

	}

}


