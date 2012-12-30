#ifndef  _Animation_LayerNode__H__
#define  _Animation_LayerNode__H__

#include "Animation/AnimationTree/AnimTreeNode.h"

namespace ma
{
	struct AnimEvalContext;

	class ANIMATION_API AnimLayerNode : public AnimTreeNode
	{
	public:
		virtual void AddLayer(AnimTreeNode* pAnimTreeNode);

		virtual void RemoveLayer(AnimTreeNode* pAnimTreeNode);

		virtual void AdvanceTime(float fTimeElapsed);

		virtual void EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight);

	private:
		std::vector<AnimTreeNode*> m_arrAnimNode;
	};
}




#endif