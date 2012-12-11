#ifndef  _Animation_ClipNode__H__
#define  _Animation_ClipNode__H__

#include "Animation/AnimationTree/AnimTreeNode.h"

namespace ma
{
	class AnimationInst;
	class AnimEvalContext;
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

	private:
		AnimationInst* m_pAnimInst;

		BoneSet* m_pBoneSet;
	};
}




#endif