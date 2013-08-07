#ifndef  _Animation_ClipNode__H__
#define  _Animation_ClipNode__H__

#include "Animation/AnimationTree/AnimTreeNode.h"

namespace ma
{
	class AnimationClip;
	struct AnimEvalContext;
	class BoneSet;

	class ANIMATION_API AnimClipNode : public AnimTreeNode
	{
	public:
		AnimClipNode();

		AnimClipNode(const char* pSkaPath,const char* pBoneSetName);
		
		~AnimClipNode();

		void			SetAnimationClip(AnimationClip* pAnimClip) {m_pAnimClip = pAnimClip;}

		void			SetBoneSet(BoneSet* pBoneSet) {m_pBoneSet = pBoneSet;}

		virtual void	AdvanceTime(float fTimeElapsed);

		virtual void	EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight);

		virtual	void	SetFrame(float fFrame);

	private:
		AnimationClip*	m_pAnimClip;

		BoneSet*		m_pBoneSet;
	};
}




#endif