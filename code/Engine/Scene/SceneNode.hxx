#include "SceneNode.h"
#include "Component.h"
#include "CullTree.h"

namespace ma
{
	IMPL_OBJECT(SceneNode,Object)
	
	SceneNode::SceneNode(Scene* pScene,const char* pName)
	{
		m_pScene = pScene;
		m_sName = pName ? pName : "";
		
		m_pParent = NULL;
		m_nLastVisibleFrame = 0;
	}

	SceneNode::~SceneNode()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			Component* pComp = m_arrComp[i].get();
			ObjectFactoryManager::GetInstance().DeleteObject(pComp->GetClass()->GetName(), pComp);
		}
		m_arrComp.clear();

		this->RemoveAllChild();
		
		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}
		m_pParent = NULL;
	}
 
	void SceneNode::AddComponent(ComponentPtr pComponent)
	{
		if (pComponent == NULL)
			return;

		std::vector<ComponentPtr>::iterator it = find(m_arrComp.begin(),m_arrComp.end(),pComponent);
		if (it != m_arrComp.end())
			return;

		m_arrComp.push_back(pComponent);

		pComponent->OnAddToSceneNode(this);
	}

	void SceneNode::RemoveComponent(RefPtr<Component> pComponent)
	{
		if (pComponent == NULL)
			return;

		std::vector<ComponentPtr>::iterator it = find(m_arrComp.begin(),m_arrComp.end(),pComponent);
		if (it != m_arrComp.end())
			return;

		m_arrComp.erase(it);

		pComponent->OnRemoveFromSceneNode(this);
	}


	void SceneNode::SerializeChild(Serializer& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);

		UINT nSize = (UINT)m_arrChild.size();
		sl.Serialize(nSize,"size");

		if (nSize != m_arrChild.size())
		{
			m_arrChild.resize(nSize);
		}

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			if (m_arrChild[nCnt] == NULL)
			{
				m_arrChild[nCnt] = new SceneNode(m_pScene);
				m_arrChild[nCnt]->SetParent(this);
			}
			sl.Serialize(*(m_arrChild[nCnt]),buf);
		}

		sl.EndSection();
	}

	void SceneNode::SerializeComp(Serializer& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);

		UINT nSize = m_arrComp.size();
		sl.Serialize(nSize,"size");

		if (nSize != m_arrComp.size())
		{
			m_arrComp.resize(nSize);
		}

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			
			Component* pComp = m_arrComp[nCnt].get();

			SerializeObjectArg<Component>(sl,pComp,this,buf);

			m_arrComp[nCnt] = pComp;
		}

		sl.EndSection();
	}

	void SceneNode::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sName,"m_sName");

		//sl.Serialize(*m_pNodeTransform,"SceneNode");
		NodeTransform::Serialize(sl,"NodeTransform");

		SerializeComp(sl,"arrComp");

		SerializeChild(sl,"arrChild");

		sl.EndSection();
	}

	void SceneNode::SetUserData(const char* pszKey,void* pData)
	{
		m_mapUserData[pszKey] = pData;
	}

	void* SceneNode::GetUserData(const char* pszKey)
	{
		return m_mapUserData[pszKey];
	}

	SceneNodePtr SceneNode::Clone(const char* pName)
	{
		XMLOutputSerializer xmlout;
		this->Serialize(xmlout);
		xmlout.Save("DebugCloneobj.xml");
		
		XMLInputSerializer xmlin;
		xmlin.Open(xmlout);
		SceneNodePtr pClone = m_pScene->CreateNode("");
		pClone->Serialize(xmlin);
		
		pClone->SetName(pName);

		return pClone;
	}

	void SceneNode::SetParent(SceneNode* pParent)
	{
		if (m_pParent == pParent)
		{
			return;
		}

		m_pParent = pParent;
	}

	void SceneNode::AddChild(SceneNodePtr pChild)
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

	void SceneNode::RemoveAllChild()
	{
		for (std::vector<SceneNodePtr>::iterator iter = m_arrChild.begin();iter != m_arrChild.end();iter++)
		{
			SceneNodePtr pGameObject = *iter;
			pGameObject->SetParent(NULL);
		}
		m_arrChild.clear();
	}

	void SceneNode::RemoveChild(SceneNodePtr pChild)
	{
		if (pChild == NULL)	
		{
			return;
		}

		std::vector<SceneNodePtr>::iterator iter = find(m_arrChild.begin(), m_arrChild.end(), pChild);
		if (iter == m_arrChild.end())
		{
			return;
		}

		m_arrChild.erase(iter);
		pChild->SetParent(NULL);

	}

	void SceneNode::OnTransformChange()
	{
		for (UINT i = 0; i < m_arrChild.size(); ++i)
		{
			m_arrChild[i]->OnTransformChange();
		}

		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{		
			m_arrComp[i]->OnTransformChange();
		}
	}

}


