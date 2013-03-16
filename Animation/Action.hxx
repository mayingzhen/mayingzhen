#include "Animation/Action.h"
#include "Animation/PoseModifier/PoseModifier.h"

namespace ma
{
	Action::Action()
	{
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
		for (UINT i = 0; i < pAnimContext->m_pNodePos->GetNodeNumber(); ++i)
		{
		 	maNodeTransform tsfPS;
		 	maTransformMul(&tsfPS,&pAnimContext->m_arrTSFLS[i],&pAnimContext->m_refNodePos->GetTransformPS(i));
		 	pAnimContext->m_pNodePos->SetTransformPS(&tsfPS,i);
		}
		 

		for (UINT i = 0; i < m_arrPoseModifier.size(); ++i)
		{
			if (m_arrPoseModifier[i] == NULL)
				continue;

			m_arrPoseModifier[i]->UpdatePose(pAnimContext->m_pNodePos);
		}

		pAnimContext->m_pNodePos->SyncObjectSpace();

	}

	void Action::Serialize(SerializeListener& sl, const char* pszLable)
	{

	}
}
