#include "Animation/AnimationTree/AnimLayerNode.h"

namespace ma
{
	void AnimLayerNode::AddLayer(AnimTreeNode* pAnimTreeNode)
	{
		if (pAnimTreeNode == NULL)
			return;

		std::vector<AnimTreeNode*>::iterator it = find(m_arrAnimNode.begin(),m_arrAnimNode.end(),pAnimTreeNode);
		if ( it != m_arrAnimNode.end() )
			return;
		m_arrAnimNode.push_back(pAnimTreeNode);
	}

	void AnimLayerNode::RemoveLayer(AnimTreeNode* pAnimTreeNode)
	{
		if (pAnimTreeNode == NULL)
			return;

		std::vector<AnimTreeNode*>::iterator it = find(m_arrAnimNode.begin(),m_arrAnimNode.end(),pAnimTreeNode);
		if ( it == m_arrAnimNode.end() )
			return;
		AnimTreeNode* pTemp = *it;
		m_arrAnimNode.erase(it);
		SAFE_DELETE(pTemp);
	}

	void AnimLayerNode::AdvanceTime(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
		{
			m_arrAnimNode[i]->AdvanceTime(fTimeElapsed);
		}
	}

	void AnimLayerNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight)
	{
		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
		{
			m_arrAnimNode[i]->EvaluateAnimation(pEvalContext,1.0f);
		}
	}

}

