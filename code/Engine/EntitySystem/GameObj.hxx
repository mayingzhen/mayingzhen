#include "GameObj.h"
#include "Component.h"
#include "CullTree.h"

namespace ma
{
	IMPL_OBJECT(GameObject,Object)
	
	GameObject::GameObject(const char* pName)
	{
		m_sName = pName ? pName : "";
		m_pScenNode = new SceneNode(this);
		m_pPhyscisObject = GetPhysicsSystem() ? GetPhysicsSystem()->CreatePhysicsObject(this) : NULL;
		
		m_pParent = NULL;
		m_pCallback = NULL;
		m_pCullNode = NULL;
		m_pCullTree = NULL;
	}

	GameObject::~GameObject()
	{
		SAFE_DELETE(m_pScenNode);
		
		if (GetPhysicsSystem() && m_pPhyscisObject)
		{
			GetPhysicsSystem()->DeletePhysicsObject(m_pPhyscisObject);
			m_pPhyscisObject = NULL;
		}

		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			Component* pComp = m_arrComp[i];
			ObjectFactoryManager::GetInstance().DeleteObject(pComp->GetClass()->GetName(), pComp);
		}
		m_arrComp.clear();

		m_pCullTree = NULL;
		
		this->RemoveAllChild();
		
		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}
		m_pParent = NULL;

		if (m_pParent)
		{
			m_pCallback->OnNodeDestroyed(this);
			m_pCallback == NULL;
		}
		m_pCullNode = NULL;
	}

	void GameObject::Update()
	{
// 		if (this->BeginMatrix())
// 		{
// 			this->CalcMatrix();
// 			this->EndMatrix();
// 		}
		m_pScenNode->GetWorldMatrix();

		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->Update();
		}

		UINT nAmount = m_arrChild.size();
		for (int i = nAmount - 1; i >= 0; --i)
		{
			GameObjectPtr& node = m_arrChild[i];
			node->Update();
		}

		if (m_pCallback)
		{
			m_pCallback->OnNodeProcessed(this);
		}
	}

	void GameObject::BeginShow(Camera* pCamera)
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{		
			m_arrComp[i]->BeginShow(pCamera);
		}

		if (m_pCallback)
		{
			m_pCallback->OnNodeBeginShow(this);
		}
	}

	void GameObject::Show(Camera* pCamera, bool bCascade)
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{		
			m_arrComp[i]->Show(pCamera);
		}

		if (bCascade)
		{
			for (UINT i = 0; i < m_arrChild.size(); ++i)
			{
				m_arrChild[i]->Show(pCamera, bCascade);
			}
		}
	}

	void GameObject::EndShow(Camera* pCamera)
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{		
			m_arrComp[i]->EndShow(pCamera);
		}

		if (m_pCallback)
		{
			m_pCallback->OnNodeEndShow(this);
		}
	}


// 	void GameObject::SetNeedChange(CHANGE_TYPE eChangeType)
// 	{
// 		m_nNeedChange |= eChangeType;
// 
// 		for (std::vector<GameObjectPtr>::iterator iter = m_arrChild.begin();iter != m_arrChild.end();iter++)
// 		{
// 			(*iter)->SetNeedChange(eChangeType);
// 		}
// 	}

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

		for (UINT i = 0; i < m_arrChild.size(); ++i)
		{
			bTraveling = m_arrChild[i]->TravelScene(pVisiter);	
			if (!bTraveling)
			{
				break;
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


	void GameObject::SerializeChild(Serializer& sl, std::vector<GameObjectPtr>& arrChild,GameObject* pParent,const char* pszLable)
	{
		sl.BeginSection(pszLable);

		UINT nSize = (UINT)arrChild.size();
		sl.Serialize(nSize,"size");

		if (nSize != arrChild.size())
		{
			arrChild.resize(nSize);
		}

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			if (arrChild[nCnt] == NULL)
			{
				arrChild[nCnt] = new GameObject();
				arrChild[nCnt]->SetParent(pParent);
				//pParent->AddChild(arrChild[nCnt]);
			}
			sl.Serialize(*(arrChild[nCnt]),buf);
		}

		sl.EndSection();
	}

	void GameObject::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sName,"m_sName");

		sl.Serialize(*m_pScenNode,"SceneNode");

		SerializeArrObjArg<Component>(sl,m_arrComp,this,"arrComp");

		SerializeChild(sl,m_arrChild,this,"arrChild");

		sl.EndSection();
	}

	GameObjectPtr GameObject::Clone(const char* pName)
	{
		XMLOutputArchive xmlout;
		this->Serialize(xmlout);
		xmlout.Save("DebugCloneobj.xml");
		
		XMLInputArchive xmlin;
		xmlin.Open(xmlout);
		GameObjectPtr pClone = GetEntitySystem()->CreateGameObject("");
		pClone->Serialize(xmlin);
		
		pClone->SetName(pName);

		return pClone;
	}

	void GameObject::SetCullTree(CullTree* pCullTree)
	{
		if (m_pCullTree == pCullTree)
		{
			return;
		}

		UINT nAmount = m_arrChild.size();
		for (int i = nAmount - 1; i >= 0; --i)
		{
			GameObjectPtr& node = m_arrChild[i];
			node->SetCullTree(pCullTree);
		}

		if (pCullTree == NULL)
		{
			m_pCullTree->RemoveObject(this);
		}

		m_pCullTree = pCullTree;
	}


	void GameObject::SetParent(GameObject* pParent)
	{
		if (m_pParent == pParent)
		{
			return;
		}

		m_pParent = pParent;
		if (m_pParent != NULL)
		{
			this->SetCullTree(m_pParent->GetCullTree());
			//this->SetNeedChange(CT_FROMPARENT);

			if (m_pCallback)
			{
				m_pCallback->OnNodeAttached(this);
			}
		}
		else
		{
			this->SetCullTree(NULL);
			if (m_pCallback)
			{
				m_pCallback->OnNodeDetached(this);
			}
		}
	}

	void GameObject::AddChild(GameObjectPtr pChild)
	{
		if (pChild == NULL)
			return;
	
		if (pChild->GetParent() == this)
			return;

		if (pChild->GetParent() != NULL)
		{
			pChild->GetParent()->RemoveChild(pChild);
		}

		ASSERT(pChild->GetParent() == NULL);

		m_arrChild.push_back(pChild);
		pChild->SetParent(this);
	}

	void GameObject::RemoveAllChild()
	{
		for (std::vector<GameObjectPtr>::iterator iter = m_arrChild.begin();iter != m_arrChild.end();iter++)
		{
			GameObjectPtr pGameObject = *iter;
			pGameObject->SetParent(NULL);
		}
		m_arrChild.clear();
	}

	void GameObject::RemoveChild(GameObjectPtr pChild)
	{
		if (pChild == NULL)
		{
			return;
		}

		std::vector<GameObjectPtr>::iterator iter = find(m_arrChild.begin(), m_arrChild.end(), pChild);
		if (iter == m_arrChild.end())
		{
			return;
		}

		m_arrChild.erase(iter);
		pChild->SetParent(NULL);

	}

	void GameObject::UpdateAABB()
	{

	}

}


