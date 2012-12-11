#include "Animation/SkeletonAnimation.h"

namespace ma
{
	SkeletonAnimation::SkeletonAnimation()
	{
		m_pAnimaNode = NULL;
	}

	SkeletonAnimation::~SkeletonAnimation()
	{
		SAFE_DELETE(m_pAnimaNode);
	}

	void SkeletonAnimation::SetTreeNode(AnimTreeNode* pAnimNode)
	{
		m_pAnimaNode = pAnimNode;
	}

	void SkeletonAnimation::AdvanceTime(float fTimeElepse)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->AdvanceTime(fTimeElepse);
		}
	}

	void SkeletonAnimation::EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->EvaluateAnimation(pAnimContext,fWeight);
		}
	}
}
