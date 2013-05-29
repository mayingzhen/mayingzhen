#include "Animation/AnimationTree/AnimBlendNode.h"


namespace ma
{
	AnimBlendNode::AnimBlendNode()
	{
		m_pSrcAnimNode = NULL;
		m_pDestAnimNode = NULL;
		m_fWeight = 0;
	}

	AnimBlendNode::~AnimBlendNode()
	{
		SAFE_DELETE(m_pSrcAnimNode);
		SAFE_DELETE(m_pDestAnimNode);
	}

	void AnimBlendNode::AdvanceTime(float fTimeElapsed)
	{
		if (m_pSrcAnimNode)
			m_pSrcAnimNode->AdvanceTime(fTimeElapsed);

		if (m_pDestAnimNode)
			m_pDestAnimNode->AdvanceTime(fTimeElapsed);
	}

	void AnimBlendNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight)
	{
		float fSrcWeight = fWeight * ( 1.0f - m_fWeight) ;
		float fDestWeight = fWeight * m_fWeight;
		if (m_pSrcAnimNode)
		{
			m_pSrcAnimNode->EvaluateAnimation(pEvalContext,fSrcWeight);
		}

		if (m_pDestAnimNode)
		{
			m_pDestAnimNode->EvaluateAnimation(pEvalContext,fDestWeight);
		}
	}
}


