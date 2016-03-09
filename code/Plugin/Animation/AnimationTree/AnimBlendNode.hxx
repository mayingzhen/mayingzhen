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

// 	void AnimBlendNode::Serialize(Serializer& sl, const char* pszLable/* = "AnimBlendNode" */)
// 	{
// 		sl.BeginSection(pszLable);
// 
// 		SerializeObject<AnimTreeNode>(sl,m_pSrcAnimNode,"SrcAnimNode");
// 		SerializeObject<AnimTreeNode>(sl,m_pSrcAnimNode,"DestAnimNode");
// 		sl.Serialize(m_fWeight,"Weight");
// 
// 		sl.EndSection();
// 	}


}


