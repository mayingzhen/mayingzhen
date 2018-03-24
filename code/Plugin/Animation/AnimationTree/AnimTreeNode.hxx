#include "AnimTreeNode.h"
#include "FrameEvent.h"
#include "../PoseModifier/PoseModifier.h"


namespace ma
{
	AnimTreeNode::AnimTreeNode()
	{
		m_fFadeTime = 0.3f;
		m_pCallBack = NULL;
		m_nAnimID = -1;
	}

	AnimTreeNode::~AnimTreeNode()
	{

	}

	void AnimTreeNode::RegisterAttribute()
	{
		ACCESSOR_ATTRIBUTE(AnimTreeNode, "Name", GetName, SetName, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimTreeNode, "FadeTime", GetFadeTime, SetFadeTime, float, 0.3f, AM_DEFAULT);
	}

	uint32 AnimTreeNode::AnimNameToID(const char* pszName)
	{
		ASSERT(pszName != NULL);
		if (pszName == NULL)
			return -1;

		int nAnimID = StringConverter::parseInt(pszName);
		if (nAnimID > 0)
		{
			return nAnimID; 
		}
		else
		{
			return Math::FastHash( pszName, strlen(pszName) );
		}
	}

	const char*	AnimTreeNode::GetName() const
	{
		return m_strName.c_str();
	}

	void AnimTreeNode::SetName(const char* pszName)
	{
		m_strName = pszName ? pszName : "";

		m_nAnimID = AnimNameToID(m_strName.c_str());
	}

	void AnimTreeNode::AddPoseModifier(PoseModifier* pPoseModifier)
	{
		m_vecPoseModifier.push_back(pPoseModifier);
	}

	void AnimTreeNode::ProcessPoseModifier(SkeletonPose* pNodePose, Skeleton* pSkeleton, float fWeight)
	{
		for (uint32 i = 0; i < m_vecPoseModifier.size(); ++i)
		{
			m_vecPoseModifier[i]->UpdatePose(pNodePose,pSkeleton,fWeight);
		}
	}

	BoneIndex AnimTreeNode::GetBoneIndex(const char *boneName) const
	{
		if (m_animator->GetSkeleton() == NULL)
			return -1;

		return m_animator->GetSkeleton()->GetBoneIdByName(boneName);
	}

	UINT AnimTreeNode::GetBoneCount() const
	{
		if (m_animator->GetSkeleton() == NULL)
			return 0;

		return m_animator->GetSkeleton()->GetBoneNumer();
	}

	void AnimTreeNode::Init(AnimationComponent* pAnimator)
	{
		m_animator = pAnimator;
	}

	void AnimTreeNode::Activate() 
	{
		m_active = true; 
	}

	bool AnimTreeNode::Import(rapidxml::xml_node<>* xmlNode)
	{
		Serializable::Import(xmlNode);

		rapidxml::xml_node<>* xmlFrameEvent = xmlNode->first_node("FrameEvent");
		while (xmlFrameEvent)
		{
			const char* pszType = xmlFrameEvent->findAttribute("ClassName");
		
			RefPtr<FrameEvent> pFrameEvent = CreateObject<FrameEvent>(pszType); 
			//m_vecFrameEvent.push_back(pFrameEvent);
			
			pFrameEvent->Import(xmlFrameEvent);

			xmlFrameEvent = xmlFrameEvent->next_sibling("FrameEvent");
		}

		rapidxml::xml_node<>* xmlPoseModifier = xmlNode->first_node("PoseModifier");
		while (xmlPoseModifier)
		{
			const char* pszType = xmlPoseModifier->findAttribute("ClassName");

			RefPtr<PoseModifier> pPoseModifier = CreateObject<PoseModifier>(pszType); 
			m_vecPoseModifier.push_back(pPoseModifier);

			pPoseModifier->Import(xmlPoseModifier);
			
			xmlPoseModifier = xmlPoseModifier->next_sibling("PoseModifier");
		}

		return true;
	}

	bool AnimTreeNode::Export(rapidxml::xml_node<>* xmlNode,rapidxml::xml_document<>& doc)
	{
		Serializable::Export(xmlNode,doc);
		
// 		for (uint32 i = 0; i < m_vecFrameEvent.size(); ++i)
// 		{
// 			rapidxml::xml_node<>* xmlFrameEvent = doc.allocate_node(rapidxml::node_element, doc.allocate_string("FrameEvent"));
// 			xmlNode->append_node(xmlFrameEvent);	
// 
// 			FrameEvent* pFrameEvent = m_vecFrameEvent[i].get();
// 
// 			pFrameEvent->Export(xmlFrameEvent,doc);
// 		}

		for (uint32 i = 0; i < m_vecPoseModifier.size(); ++i)
		{
			rapidxml::xml_node<>* xmlPoseModifier = doc.allocate_node(rapidxml::node_element, doc.allocate_string("PoseModifier"));
			xmlNode->append_node(xmlPoseModifier);	

			PoseModifier* pPoseModifier = m_vecPoseModifier[i].get();
		
			pPoseModifier->Export(xmlPoseModifier,doc);
		}

		return true;
	}
}