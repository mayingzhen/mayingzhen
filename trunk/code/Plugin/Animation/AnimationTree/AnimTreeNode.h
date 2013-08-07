#ifndef  _Animation_TreeNode__H__
#define  _Animation_TreeNode__H__

namespace ma
{
	struct AnimEvalContext;

	class ANIMATION_API AnimTreeNode
	{
	public:
		virtual void AdvanceTime(float fTimeElapsed) = 0;

		virtual void EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight) = 0;

		virtual	void SetFrame(float fFrame) = 0;

		Skeleton*	GetSkeleton() {return m_pSkeleton;}

		void		SetSkeleton(Skeleton* pSkeleton) {m_pSkeleton = pSkeleton;}
			
	protected:
		Skeleton*	m_pSkeleton;
	};
}




#endif