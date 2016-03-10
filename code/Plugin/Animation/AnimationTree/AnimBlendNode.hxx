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
		AnimTreeNode::AdvanceTime(fTimeElapsed);

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

	bool AnimBlendNode::Instantiate(Skeleton* pSkeletion)
	{
		if (m_pSrcAnimNode)
		{
			m_pSrcAnimNode->Instantiate(pSkeletion);
		}

		if (m_pDestAnimNode)
		{
			m_pDestAnimNode->Instantiate(pSkeletion);
		}

		return true;
	}

	bool AnimBlendNode::IsReady()
	{
		if (m_pSrcAnimNode && !m_pSrcAnimNode->IsReady())
			return false;

		if (m_pDestAnimNode && !m_pDestAnimNode->IsReady())
			return false;

		return true;
	}

	uint32 AnimBlendNode::GetFrameCount()
	{
		UINT nSrcFrameCount = 0;
		if (m_pDestAnimNode)
		{
			nSrcFrameCount = m_pDestAnimNode->GetFrameCount();
		}

		UINT nDestFrameCount = 0;
		if (m_pSrcAnimNode)
		{
			nDestFrameCount = m_pSrcAnimNode->GetFrameCount();
		}

		return max(nSrcFrameCount,nDestFrameCount);
	}

}


