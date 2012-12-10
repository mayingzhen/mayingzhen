#ifndef  _Animation_ClipNode__H__
#define  _Animation_ClipNode__H__

#include "Animation/AnimationTree/AnimTreeNode.h"

namespace ma
{
	class AnimationInst;
	class AnimEvalContext;

	class ANIMATION_API AnimClipNode : public AnimTreeNode
	{
	public:
		AnimClipNode();
		
		~AnimClipNode();

		void SetAnimationInst(AnimationInst* pAnimInst) {m_pAnimInst = pAnimInst;}

		virtual void AdvanceTime(float fTimeElapsed);

		virtual void EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight);

	private:
		AnimationInst* m_pAnimInst;
	};
}




#endif