#include "Animation/AnimationTree/AnimClipNode.h"


namespace ma
{
	AnimClipNode::AnimClipNode()
	{
		m_pAnimInst = NULL;
	}

	AnimClipNode::~AnimClipNode()
	{

	}

	void AnimClipNode::AdvanceTime(float fTimeElapsed)
	{
		if (m_pAnimInst == NULL)
			return;

		m_pAnimInst->AdvanceTime(fTimeElapsed);
	}

	void AnimClipNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight)
	{
		if (m_pAnimInst == NULL)
			return;

		m_pAnimInst->EvaluateAnimation(pEvalContext,fWeight);
	}
}
