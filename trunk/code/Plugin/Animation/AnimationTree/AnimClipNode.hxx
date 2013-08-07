#include "Animation/AnimationTree/AnimClipNode.h"


namespace ma
{
	AnimClipNode::AnimClipNode(/*const char* pAnimPath,const char* pBonsetName*/)
	{
		m_pBoneSet = NULL;
		m_pAnimClip = NULL;
		
	}

	AnimClipNode::~AnimClipNode()
	{

	}

	void AnimClipNode::AdvanceTime(float fTimeElapsed)
	{
		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->AdvanceTime(fTimeElapsed);
	}

	void AnimClipNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight)
	{
		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->EvaluateAnimation(pEvalContext,fWeight,m_pBoneSet);
	}

	void AnimClipNode::SetFrame(float fFrame)
	{
		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->SetFrame(fFrame);
	}
}
