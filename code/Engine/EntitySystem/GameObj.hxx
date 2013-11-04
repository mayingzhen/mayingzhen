#include "GameObj.h"
#include "Component.h"

namespace ma
{
	IMPL_OBJECT(GameObject,Object)
	
	GameObject::GameObject(const char* pName)
	{
		m_sName = pName;
		m_pScenNode = GetSceneSystem() ? GetSceneSystem()->CreateSceneNode(this) : NULL;
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
			Component* pComp = m_arrComp[i];
			ObjectFactoryManager::GetInstance().DeleteObject(pComp->GetClass()->GetName(), pComp);
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
		XMLOutputArchive xmlout;
		this->Serialize(xmlout);

		XMLInputArchive xmlin;
		xmlin.Open(xmlout);
		GameObject* pClone = GetEntitySystem()->CreateGameObject(pName);
		pClone->Serialize(xmlin);

		return pClone;
	}

	void GameObject::UpdateAABB()
	{

	}

}


