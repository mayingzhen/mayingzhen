#include "Animation/AnimationTree/AnimBlendNode.h"


namespace ma
{
	IMPL_OBJECT(AnimBlendNode,IAnimBlendNode)

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

	void AnimBlendNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight,EBlendMode eBlendMode)
	{
		float fSrcWeight = fWeight * ( 1.0f - m_fWeight) ;
		float fDestWeight = fWeight * m_fWeight;
		if (m_pSrcAnimNode)
		{
			m_pSrcAnimNode->EvaluateAnimation(pEvalContext,fSrcWeight,BLENDMODE_OVERWRITE);
		}

		if (m_pDestAnimNode)
		{
			m_pDestAnimNode->EvaluateAnimation(pEvalContext,fDestWeight,BLENDMODE_OVERWRITE);
		}
	}

	void AnimBlendNode::SetFrame(float fFrame)
	{
		if (m_pSrcAnimNode)
		{
			m_pSrcAnimNode->SetFrame(fFrame);
		}

		if (m_pDestAnimNode)
		{
			m_pDestAnimNode->SetFrame(fFrame);
		}
	}


}


