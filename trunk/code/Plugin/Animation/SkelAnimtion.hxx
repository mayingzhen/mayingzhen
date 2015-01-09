#include "SkelAnimtion.h"
#include "PoseModifier/PoseModifier.h"
#include "AnimationTree/AnimBlendNode.h"
#include "AnimationTree/AnimClipNode.h"
#include "AnimationTree/AnimLayerNode.h"

namespace ma
{
	SkelAnimtion::SkelAnimtion()
	{
		m_pAnimaNode = NULL;
	}

	SkelAnimtion::~SkelAnimtion()
	{
		m_pAnimaNode = NULL;
	}

	void SkelAnimtion::SetSkeletion(Skeleton* pSkeleton)
	{
		ASSERT(m_pAnimaNode);
		if (m_pAnimaNode == NULL)
			return;

		m_pSkeleton = pSkeleton;

		m_pAnimaNode->SetSkeletion(pSkeleton);
	}

	void SkelAnimtion::SetTreeNode(AnimTreeNode* pAnimNode)
	{
		m_pAnimaNode = pAnimNode;
	}

	void SkelAnimtion::AddPoseModifier(PoseModifier* pPoseModifier)
	{
		if (pPoseModifier == NULL)
			return;

		m_arrPoseModifier.push_back((PoseModifier*)pPoseModifier);
	}

	void SkelAnimtion::RemovePoseModifier(PoseModifier* pPoseModifier)
	{
		PoseModifier* pModifier = (PoseModifier*)pPoseModifier;
		if (pModifier == NULL)
			return;

		std::vector<PoseModifier*>::iterator it = std::find(m_arrPoseModifier.begin(),m_arrPoseModifier.end(),pModifier);
		if (it == m_arrPoseModifier.end())
			return;

		m_arrPoseModifier.erase(it);
	}

	void SkelAnimtion::AdvanceTime(float fTimeElepse)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->AdvanceTime(fTimeElepse);
		}
	}

	void SkelAnimtion::EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight)
	{
		profile_code();

		if (m_pAnimaNode)
		{
			m_pAnimaNode->EvaluateAnimation(pAnimContext,fWeight,BLENDMODE_OVERWRITE);
		}

		// ApplyLocalSpaceAnimation
		pAnimContext->m_pNodePos->InitLocalSpace(pAnimContext->m_arrTSFLS,pAnimContext->m_refNodePos);
		 

		for (UINT i = 0; i < m_arrPoseModifier.size(); ++i)
		{
			if (m_arrPoseModifier[i] == NULL)
				continue;

			m_arrPoseModifier[i]->UpdatePose(pAnimContext->m_pNodePos);
		}

		pAnimContext->m_pNodePos->SyncObjectSpace();
	}

	void SkelAnimtion::SetFrame(float fFrame)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->SetFrame(fFrame);
		}
	}


	AnimClipNode*	SkelAnimtion::CreateClipNode(const char* pSkaPath,const char* pBonsetName)
	{
		AnimClipNode* pClipNode = new AnimClipNode();

		pClipNode->SetAnimationClip(pSkaPath);
		pClipNode->SetBoneSet(pBonsetName);
	
		return pClipNode;
	}

	void SkelAnimtion::Serialize(Serializer& sl, const char* pszLable/* = "ActionData"*/)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sAnimName,"ActionName");

		SerializeObject<AnimTreeNode>(sl,m_pAnimaNode,"AnimationNode");

		sl.EndSection();
	}

	bool SkelAnimtion::OnLoadOver()
	{
		if (m_pAnimaNode && !m_pAnimaNode->OnLoadOver())
			return false;

		return true;
	}
}

