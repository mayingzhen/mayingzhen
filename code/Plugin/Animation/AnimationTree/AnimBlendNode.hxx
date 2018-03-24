#include "AnimBlendNode.h"


namespace ma
{
	AnimBlendNode::AnimBlendNode()
	{
		m_fWeight = 0;
	}

	AnimBlendNode::~AnimBlendNode()
	{
	}

	void AnimBlendNode::AdvanceTime(float fTimeElapsed)
	{
		if (m_pSrcAnimNode)
			m_pSrcAnimNode->AdvanceTime(fTimeElapsed);

		if (m_pDestAnimNode)
			m_pDestAnimNode->AdvanceTime(fTimeElapsed);
	}

	void AnimBlendNode::EvaluateAnimation(AnimationNodeOutput &output, float fWeight)
	{
		float fSrcWeight = fWeight * ( 1.0f - m_fWeight) ;
		float fDestWeight = fWeight * m_fWeight;
		if (m_pSrcAnimNode)
		{
			m_pSrcAnimNode->EvaluateAnimation(output,fSrcWeight);
		}

		if (m_pDestAnimNode)
		{
			m_pDestAnimNode->EvaluateAnimation(output,fDestWeight);
		}
	}

	void AnimBlendNode::Activate()
	{
		if (m_pSrcAnimNode)
		{
			m_pSrcAnimNode->Activate();
		}

		if (m_pDestAnimNode)
		{
			m_pDestAnimNode->Activate();
		}
	}

	bool AnimBlendNode::IsReady()
	{
		if (m_pSrcAnimNode && !m_pSrcAnimNode->IsReady())
			return false;

		if (m_pDestAnimNode && !m_pDestAnimNode->IsReady())
			return false;

		return true;
	}

}




