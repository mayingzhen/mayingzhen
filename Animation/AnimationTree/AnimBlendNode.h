#ifndef  _Animation_BlendNode__H__
#define  _Animation_BlendNode__H__

#include "Animation/AnimationTree/AnimTreeNode.h"

namespace ma
{
	class AnimEvalContext;

	class ANIMATION_API AnimBlendNode : public AnimTreeNode
	{
	public:
		AnimBlendNode();
		
		~AnimBlendNode();

		void SetSrcAnimNode(AnimTreeNode* pAnimNode) {m_pSrcAnimNode = pAnimNode;}

		void SetDestAnimNode(AnimTreeNode* pAnimNode) {m_pDestAnimNode = pAnimNode;}

		void SetWeight(float fWeight) { m_fWeight = fWeight;}

		virtual void AdvanceTime(float fTimeElapsed);

		virtual void EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight);

	private:
		AnimTreeNode* m_pSrcAnimNode;
		AnimTreeNode* m_pDestAnimNode;
		float		  m_fWeight;
		
	};
}




#endif