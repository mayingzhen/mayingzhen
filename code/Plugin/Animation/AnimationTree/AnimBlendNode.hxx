#include "AnimBlendNode.h"


namespace ma
{
	IMPL_OBJECT(AnimBlendNode,AnimTreeNode)

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

	void AnimBlendNode::SetSkeletion(Skeleton* pSkeletion)
	{
		if (m_pSrcAnimNode)
		{
			m_pSrcAnimNode->SetSkeletion(pSkeletion);
		}

		if (m_pDestAnimNode)
		{
			m_pDestAnimNode->SetSkeletion(pSkeletion);
		}
	}

	bool AnimBlendNode::Instantiate()
	{
		if (m_pSrcAnimNode)
		{
			m_pSrcAnimNode->Instantiate();
		}

		if (m_pDestAnimNode)
		{
			m_pDestAnimNode->Instantiate();
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


