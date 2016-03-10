#include "AnimationSet.h"
#include "AnimationComponent.h"
#include "PoseModifier/PoseModifier.h"

namespace ma
{
	AnimationSet::AnimationSet()
	{
	}

	AnimationSet::~AnimationSet()
	{
	}

	AnimTreeNode* AnimationSet::GetAnimationByIndex(UINT index)
	{
		if (index < 0 || index >= m_arrSkelAnim.size())
			return NULL;

		return m_arrSkelAnim[index].get();
	}

	AnimTreeNode* AnimationSet::GetSkelAnimByName(const char* pszName)
	{
		return GetAnimationByAnimID( AnimTreeNode::AnimNameToID(pszName) );
	}

	AnimTreeNode* AnimationSet::GetAnimationByAnimID(UINT nAnimID)
	{
		ASSERT(nAnimID != -1);
		if (nAnimID == -1)
			return NULL;

		for (UINT i = 0; i < m_arrSkelAnim.size(); ++i)
		{
			if (m_arrSkelAnim[i]->GetAnimID() == nAnimID)
			{
				return m_arrSkelAnim[i].get();
			}
		}

		return NULL;
	}

	void AnimationSet::AddSkelAnim(AnimTreeNode* pAction)
	{
		if (pAction == NULL)
			return;

		if (pAction->GetAnimID() == -1)
		{
			ASSERT(false&&"GetAnimID == -1");
			return;
		}

		for (uint32 i = 0; i < m_arrSkelAnim.size(); ++i)
		{
			if (m_arrSkelAnim[i] == pAction || m_arrSkelAnim[i]->GetAnimID() == pAction->GetAnimID() )
			{
				ASSERT(false&&"存在相同的AnimNode");
				return;
			}
		}

		m_arrSkelAnim.push_back(pAction);
	}

	void AnimationSet::RemoveSkelAnim(AnimTreeNode* pAction)
	{
		if (pAction == NULL)
			return;

		VEC_SkELANIM::iterator it = std::find(m_arrSkelAnim.begin(),m_arrSkelAnim.end(),pAction);
		if (it == m_arrSkelAnim.end())
			return;

		m_arrSkelAnim.erase(it);
	}

	bool AnimationSet::Import(rapidxml::xml_node<>* pXmlElem)
	{
		rapidxml::xml_node<>* pXmlAnimNode = pXmlElem->first_node("AnimNode");
		while(pXmlAnimNode)
		{
			const char* pszType = pXmlAnimNode->findAttribute("ClassName");

			RefPtr<AnimTreeNode> pAnimNode = CreateObject<AnimTreeNode>(pszType);
			this->AddSkelAnim(pAnimNode.get());

			pAnimNode->Import(pXmlAnimNode);

			pXmlAnimNode = pXmlAnimNode->next_sibling("AnimNode");
		}

		return true;
	}

	bool AnimationSet::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		for (UINT i = 0; i < m_arrSkelAnim.size(); ++i)
		{
			rapidxml::xml_node<>* pXmlAnimNode = doc.allocate_node(rapidxml::node_element, doc.allocate_string("AnimNode"));
			pXmlElem->append_node(pXmlAnimNode);

			AnimTreeNode* pAnimNode = m_arrSkelAnim[i].get();
			pAnimNode->Export(pXmlAnimNode,doc);
		}

		return true;
	}

	RefPtr<AnimationSet> CreateAnimationSet()
	{
		return new AnimationSet();
	}

	RefPtr<AnimationSet> CreateAnimationSet(const char* pszFile)
	{
		RefPtr<AnimationSet> pAnimSet = new AnimationSet();
		pAnimSet->LoadFromXML(pszFile);
		return pAnimSet;
	}
}

