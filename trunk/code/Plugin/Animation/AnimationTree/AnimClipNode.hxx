#include "AnimClipNode.h"


namespace ma
{
	IMPL_OBJECT(AnimClipNode,AnimTreeNode)
	
	AnimClipNode::AnimClipNode()
	{
		m_bClipInit = false;
	}

	AnimClipNode::~AnimClipNode()
	{
	}

	void AnimClipNode::RegisterObject(Context* context)
	{
		ACCESSOR_ATTRIBUTE(AnimClipNode, "AnimationClip", GetAnimationClip, SetAnimationClip, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimClipNode, "BoneSet", GetBoneSet, SetBoneSet, const char*, NULL, AM_DEFAULT);
	}

	void AnimClipNode::AdvanceTime(float fTimeElapsed)
	{
		ASSERT(m_pAnimClip);
		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->AdvanceTime(fTimeElapsed);
	}

	void AnimClipNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight, EBlendMode eBlendMode)
	{
		profile_code();

		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->EvaluateAnimation(pEvalContext,fWeight,m_pBoneSet.get(),eBlendMode);
	}

	void AnimClipNode::SetFrame(float fFrame)
	{
		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->SetFrame(fFrame);
	}

	void AnimClipNode::SetSkeletion(Skeleton* pSkeletion)
	{
		m_pSkeleton = pSkeletion;
	
		if (!m_strBoneSetName.empty())
			m_pBoneSet = pSkeletion->GetBoneSetByName(m_strBoneSetName.c_str());

		if (m_pAnimClip)
			m_pAnimClip->SetSkeleton(pSkeletion);
	}
	
	const char*	AnimClipNode::GetBoneSet() const
	{
		return m_pBoneSet ? m_pBoneSet->GetBoneSetName() : NULL;
	}

	void AnimClipNode::SetBoneSet(const char* pBoneSetName)
	{
		m_strBoneSetName = pBoneSetName ? pBoneSetName : "";
	
		if (m_pSkeleton)
			m_pBoneSet = m_pSkeleton->GetBoneSetByName(pBoneSetName);

	}

	void AnimClipNode::SetAnimationClip(const char* pszSkaPath)
	{
		m_pAnimClip = new AnimationClip();
		m_pAnimClip->SetAnimationPath(pszSkaPath);
	}

	const char*	AnimClipNode::GetAnimationClip() const
	{
		return m_pAnimClip ? m_pAnimClip->GetAnimationPath() : NULL;
	}

	bool AnimClipNode::OnLoadOver()
	{
		if (!m_bClipInit)
		{
			m_pAnimClip->SetSkeleton(m_pSkeleton.get());
			m_pAnimClip->Instance();
			m_bClipInit = true;
		}

		if (m_pAnimClip && !m_pAnimClip->OnLoadOver())
			return false;
		
		return true;
	}

	RefPtr<AnimClipNode>  CreateClipNode(const char* skaName,const char* boneSetName)
	{
		AnimClipNode* pClipNode = new AnimClipNode();
		pClipNode->SetAnimationClip(skaName);
		pClipNode->SetBoneSet(boneSetName);
		return pClipNode;
	}

}
