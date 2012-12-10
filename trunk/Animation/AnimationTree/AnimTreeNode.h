#ifndef  _Animation_TreeNode__H__
#define  _Animation_TreeNode__H__

namespace ma
{
	class AnimEvalContext;

	class ANIMATION_API AnimTreeNode
	{
	public:
		virtual void AdvanceTime(float fTimeElapsed) = 0;

		virtual void EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight) = 0;
	};
}




#endif