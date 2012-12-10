#ifndef  _SkeletonAnimation__H__
#define  _SkeletonAnimation__H__

namespace ma
{
	class AnimTreeNode;

	// 合成动作（动作树+IK）
	class ANIMATION_API CompoundAnimation
	{
	public:
		CompoundAnimation();

		~CompoundAnimation();

		void SetTreeNode(AnimTreeNode* pAnimNode);

		void AdvanceTime(float fTimeElepse);

		void BeginAnimEval(AnimEvalContext* pAnimContext);

		void EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight);

		void EndAnimEval(AnimEvalContext* pAnimContext);

	private:
		AnimTreeNode* m_pAnimaNode;

		//std::vector<>
	};
}

#endif