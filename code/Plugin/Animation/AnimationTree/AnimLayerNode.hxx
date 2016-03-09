#include "AnimLayerNode.h"

namespace ma
{
	void AnimLayerNode::AddLayer(AnimTreeNode* pAnimTreeNode)
	{
		if (pAnimTreeNode == NULL)
			return;

		VEC_ANIMNODE::iterator it = find(m_arrAnimNode.begin(),m_arrAnimNode.end(),pAnimTreeNode);
		if ( it != m_arrAnimNode.end() )
			return;

		m_arrAnimNode.push_back(pAnimTreeNode);
	}

	void AnimLayerNode::RemoveLayer(AnimTreeNode* pAnimTreeNode)
	{
		if (pAnimTreeNode == NULL)
			return;

		VEC_ANIMNODE::iterator it = find(m_arrAnimNode.begin(),m_arrAnimNode.end(),pAnimTreeNode);
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

	void AnimLayerNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight)
	{
		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
		{
			m_arrAnimNode[i]->EvaluateAnimation(pEvalContext,fWeight);
		}
	}

	void AnimLayerNode::SetFrame(float fFrame)
	{
		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
		{
			m_arrAnimNode[i]->SetFrame(fFrame);
		}
	}

	bool AnimLayerNode::Instantiate(Skeleton* pSkeletion)
	{
		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
		{
			m_arrAnimNode[i]->Instantiate(pSkeletion);
		}

		return true;
	}

	bool AnimLayerNode::IsReady()
	{
		for (UINT i = 0; i < m_arrAnimNode.size(); ++i)
		{
			if ( !m_arrAnimNode[i]->IsReady() )
				return false;
		}

		return true;
	}

	RefPtr<AnimLayerNode> CreateLayerNode()
	{
		return new AnimLayerNode();
	}

}

