#include "Action.h"
#include "PoseModifier/PoseModifier.h"
#include "AnimationTree/AnimBlendNode.h"
#include "AnimationTree/AnimClipNode.h"
#include "AnimationTree/AnimLayerNode.h"

namespace ma
{
	Action::Action(const char* pName)
	{
		m_sAnimName = pName ? pName : "";
		m_pAnimaNode = NULL;
	}

	Action::~Action()
	{
		SAFE_DELETE(m_pAnimaNode);
	}

	void Action::SetTreeNode(IAnimTreeNode* pAnimNode)
	{
		m_pAnimaNode = pAnimNode;
	}

	void Action::AddPoseModifier(IPoseModifier* pPoseModifier)
	{
		if (pPoseModifier == NULL)
			return;

		m_arrPoseModifier.push_back((PoseModifier*)pPoseModifier);
	}

	void Action::RemovePoseModifier(IPoseModifier* pPoseModifier)
	{
		PoseModifier* pModifier = (PoseModifier*)pPoseModifier;
		if (pModifier == NULL)
			return;

		std::vector<PoseModifier*>::iterator it = std::find(m_arrPoseModifier.begin(),m_arrPoseModifier.end(),pModifier);
		if (it == m_arrPoseModifier.end())
			return;

		m_arrPoseModifier.erase(it);
	}

	void Action::AdvanceTime(float fTimeElepse)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->AdvanceTime(fTimeElepse);
		}
	}

	void Action::EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->EvaluateAnimation(pAnimContext,fWeight);
		}

		// ApplyLocalSpaceAnimation
		UINT nBoneNumber = pAnimContext->m_pNodePos->GetNodeNumber();
		std::vector<NodeTransform> arrTSfPS;
		arrTSfPS.resize(nBoneNumber);
		for (UINT i = 0; i < nBoneNumber; ++i)
		{
		 	TransformMul(&arrTSfPS[i],&pAnimContext->m_arrTSFLS[i],&pAnimContext->m_refNodePos->GetTransformPS(i));
		}
		pAnimContext->m_pNodePos->SetTransformPSAll(arrTSfPS);
		 

		for (UINT i = 0; i < m_arrPoseModifier.size(); ++i)
		{
			if (m_arrPoseModifier[i] == NULL)
				continue;

			m_arrPoseModifier[i]->UpdatePose(pAnimContext->m_pNodePos);
		}

		pAnimContext->m_pNodePos->SyncObjectSpace();

	}

	void Action::SetFrame(float fFrame)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->SetFrame(fFrame);
		}
	}

	void Action::Serialize(Serializer& sl, const char* pszLable)
	{

	}

	IAnimLayerNode*		Action::CreateLayerNode()
	{
		return new AnimLayerNode();
	}

	IAnimClipNode*		Action::CreateClipNode(const char* pSkaPath,const char* pBonsetNam)
	{
		AnimClipNode* pClipNode = new AnimClipNode();
		pClipNode->SetSkeleton(m_pSkeleton);
		pClipNode->SetAnimationClip(pSkaPath);
		pClipNode->SetBoneSet(pBonsetNam);
		return pClipNode;
	}

	IAnimBlendNode*		Action::CreateBlendNode()
	{
		return new AnimBlendNode();
	}
}

