#include "Animation/CompoundAnimationn.h"

namespace ma
{
	CompoundAnimation::CompoundAnimation()
	{
		m_pAnimaNode = NULL;
	}

	CompoundAnimation::~CompoundAnimation()
	{
		SAFE_DELETE(m_pAnimaNode);
	}

	void CompoundAnimation::SetTreeNode(AnimTreeNode* pAnimNode)
	{
		m_pAnimaNode = pAnimNode;
	}

	void CompoundAnimation::AdvanceTime(float fTimeElepse)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->AdvanceTime(fTimeElepse);
		}
	}

	void CompoundAnimation::BeginAnimEval(AnimEvalContext* pAnimContext)
	{
		
	}

	void CompoundAnimation::EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight)
	{
		if (m_pAnimaNode)
		{
			m_pAnimaNode->EvaluateAnimation(pAnimContext,fWeight);
		}
	}

	void CompoundAnimation::EndAnimEval(AnimEvalContext* pAnimContext)
	{
	
	}
}
