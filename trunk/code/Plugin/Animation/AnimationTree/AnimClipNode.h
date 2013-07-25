#ifndef  _Animation_ClipNode__H__
#define  _Animation_ClipNode__H__

#include "Animation/AnimationTree/AnimTreeNode.h"

namespace ma
{
	class AnimationInst;
	struct AnimEvalContext;
	class BoneSet;

	class ANIMATION_API AnimClipNode : public AnimTreeNode
	{
	public:
		AnimClipNode(AnimationInst* pAnimInst = NULL,BoneSet* pBoneSet = NULL);
		
		~AnimClipNode();

		void SetAnimationInst(AnimationInst* pAnimInst) {m_pAnimInst = pAnimInst;}

		void SetBoneSet(BoneSet* pBoneSet) {m_pBoneSet = pBoneSet;}

		virtual void AdvanceTime(float fTimeElapsed);

		virtual void EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight);

		virtual	void SetFrame(float fFrame);

	private:
		AnimationInst* m_pAnimInst;

		BoneSet* m_pBoneSet;
	};
}




#endif