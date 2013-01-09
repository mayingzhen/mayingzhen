#include "Animation/AnimationSet.h"

namespace ma
{
	AnimationAction* AnimationSet::GetAnimationActionByName(const char* pszName)
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
		AnimationData* pAnimData = LoadAnimationFromBinaryFile(pszAnimPath);
		pAnimation->InitWithData(pAnimData);
		pAnimation->ConverteAnimDataParentToLocalSpaceAnimation(pOriSkeleton);
		AnimationInst* pAnimationInst = new AnimationInst(pAnimation,pSkeleton);
		AnimClipNode* pClipNode = new AnimClipNode();
		pClipNode->SetAnimationInst(pAnimationInst);
		AnimationAction* pSkelAnim = new AnimationAction;
		pSkelAnim->SetAnimName(pszAnimName);
		pSkelAnim->SetTreeNode(pClipNode);

		m_arrAnimation.push_back(pSkelAnim);

		return pAnimationInst;
	}

	void AnimationSet::AddAnimationAction(AnimationAction* pSkelAnim)
	{
		if (pSkelAnim == NULL)
			return;

		m_arrAnimation.push_back(pSkelAnim);
	}

	void AnimationSet::AddAnimationInst(AnimationInst* pAnimIns,const char* pszAnimName)
	{
		AnimClipNode* pClipNode = new AnimClipNode();
		pClipNode->SetAnimationInst(pAnimIns);
		AnimationAction* pSkelAnim = new AnimationAction;
		pSkelAnim->SetAnimName(pszAnimName);
		pSkelAnim->SetTreeNode(pClipNode);

		m_arrAnimation.push_back(pSkelAnim);
	}

}

