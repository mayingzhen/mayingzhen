#include "Animation/AnimationTree/AnimClipNode.h"


namespace ma
{
	AnimClipNode::AnimClipNode()
	{
		m_pBoneSet = NULL;
		m_pAnimClip = NULL;

		m_pSkeleton = NULL;
	}

	AnimClipNode::~AnimClipNode()
	{

	}

	void AnimClipNode::AdvanceTime(float fTimeElapsed)
	{
		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->AdvanceTime(fTimeElapsed);
	}

	void AnimClipNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight)
	{
		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->EvaluateAnimation(pEvalContext,fWeight,m_pBoneSet);
	}

	void AnimClipNode::SetFrame(float fFrame)
	{
		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->SetFrame(fFrame);
	}

	void AnimClipNode::SetAnimationClip(const char* pszSkaPath)
	{
		ASSERT(m_pAnimClip == NULL);
		if (m_pAnimClip)
		{
			SAFE_DELETE(m_pAnimClip);
		}

		Animation* pAnim = DeclareResource<Animation>(pszSkaPath);
		m_pAnimClip = new AnimationClip(pAnim,m_pSkeleton);
	}

	void AnimClipNode::SetBoneSet(const char* pBoneSetName)
	{
		if (pBoneSetName == NULL)
			return;

		if (m_pSkeleton == NULL)
			return;

		BoneSet* pBoneSet = m_pSkeleton->GetBoneSetByName(pBoneSetName);
		ASSERT(pBoneSet);
		if (pBoneSet == NULL)
			return;

		m_pBoneSet = pBoneSet;

	}
}
