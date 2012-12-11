#include "Animation/AnimationSet.h"

namespace ma
{
	SkeletonAnimation* AnimationSet::GetSkeletonAnimationByName(const char* pszName)
	{
		if (pszName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrAnimation.size(); ++i)
		{
			if (m_arrAnimation[i] == NULL)
				continue;

			if (_stricmp(pszName,m_arrAnimation[i]->GetAnimName()) == 0)
				return m_arrAnimation[i];
		}

		return NULL;
	}

	AnimationInst* AnimationSet::LoadAnimation(const char* pszAnimPath,const char* pszAnimName,
		Skeleton* pSkeleton,Skeleton* pOriSkeleton)
	{
		if (pszAnimPath == NULL)
			return NULL;

		Animation* pAnimation = new Animation();
		pAnimation->Load(pszAnimPath);
		pAnimation->ConverteAnimDataParentToLocalSpaceAnimation(pOriSkeleton);
		AnimationInst* pAnimationInst = new AnimationInst(pAnimation,pSkeleton);
		AnimClipNode* pClipNode = new AnimClipNode();
		pClipNode->SetAnimationInst(pAnimationInst);
		SkeletonAnimation* pSkelAnim = new SkeletonAnimation;
		pSkelAnim->SetAnimName(pszAnimName);
		pSkelAnim->SetTreeNode(pClipNode);

		m_arrAnimation.push_back(pSkelAnim);

		return pAnimationInst;
	}

	void AnimationSet::AddSkeletonAnimation(SkeletonAnimation* pSkelAnim)
	{
		if (pSkelAnim == NULL)
			return;

		m_arrAnimation.push_back(pSkelAnim);
	}

}

