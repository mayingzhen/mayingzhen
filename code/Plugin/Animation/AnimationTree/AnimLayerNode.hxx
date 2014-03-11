#include "Animation/AnimationTree/AnimLayerNode.h"

namespace ma
{
	IMPL_OBJECT(AnimLayerNode,IAnimLayerNode)

	void AnimLayerNode::AddLayer(IAnimTreeNode* pAnimTreeNode)
	{
		if (pAnimTreeNode == NULL)
			return;

		std::vector<IAnimTreeNode*>::iterator it = find(m_arrAnimNode.begin(),m_arrAnimNode.end(),pAnimTreeNode);
		if ( it != m_arrAnimNode.end() )
			return;

		m_arrAnimNode.push_back(pAnimTreeNode);
	}

	void AnimLayerNode::RemoveLayer(IAnimTreeNode* pAnimTreeNode)
	{
		if (pAnimTreeNode == NULL)
			return;

		std::vector<IAnimTreeNode*>::iterator it = find(m_arrAnimNode.begin(),m_arrAnimNode.end(),pAnimTreeNode);
		if ( it == m_arrAnimNode.end() )
			return;

		m_arrAnimNode.erase(it);
	}

	void AnimLayerNode::AdvanceTime(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
		{
			m_arrAnimNode[i]->AdvanceTime(fTimeElapsed);
		}
	}

	void AnimLayerNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight,EBlendMode eBlendMode)
	{
		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
		{
			m_arrAnimNode[i]->EvaluateAnimation(pEvalContext,1.0f,BLENDMODE_ADDITIVE);
		}
	}

	void AnimLayerNode::SetFrame(float fFrame)
	{
		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
		{
			m_arrAnimNode[i]->SetFrame(fFrame);
		}
	}

// 	void AnimLayerNode::SetSkeleton(Skeleton* pSkeleton)
// 	{
// 		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
// 		{
// 			m_arrAnimNode[i]->SetSkeleton(pSkeleton);
// 		}
// 	}


}

