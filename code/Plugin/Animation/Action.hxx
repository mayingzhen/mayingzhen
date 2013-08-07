#include "Animation/Action.h"
#include "Animation/PoseModifier/PoseModifier.h"

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

	void Action::SetTreeNode(AnimTreeNode* pAnimNode)
	{
		m_pAnimaNode = pAnimNode;
	}

	void Action::AddPoseModifier(PoseModifier* pPoseModifier)
	{
		if (pPoseModifier == NULL)
			return;

		m_arrPoseModifier.push_back(pPoseModifier);
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
}
