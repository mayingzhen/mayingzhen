#include "AnimClipNode.h"


namespace ma
{
	IMPL_OBJECT(AnimClipNode,AnimTreeNode)
	
	AnimClipNode::AnimClipNode()
	{
		m_pBoneSet = NULL;
		m_pAnimClip = NULL;

		m_pSkeleton = NULL;

		m_bClipInit = false;

		m_pAnimClip = new AnimationClip();
	}


	AnimClipNode::~AnimClipNode()
	{
		SAFE_DELETE(m_pAnimClip);
	}

	void AnimClipNode::AdvanceTime(float fTimeElapsed)
	{
		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->AdvanceTime(fTimeElapsed);
	}

	void AnimClipNode::EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight, EBlendMode eBlendMode)
	{
		profile_code();

		if (m_pAnimClip == NULL)
			return;

		m_pAnimClip->EvaluateAnimation(pEvalContext,fWeight,m_pBoneSet,eBlendMode);
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
	
		if (m_pBoneSet == NULL)
			SetBoneSet(m_strBoneSetName.c_str());

		//if (m_pAnimClip == NULL)
		//	SetAnimationClip(m_strClipName.c_str());
	}

	void AnimClipNode::SetAnimationClip(const char* pszSkaPath)
	{
		ASSERT(m_pAnimClip == NULL);
		if (m_pAnimClip)
		{
			SAFE_DELETE(m_pAnimClip);
		}

		m_pAnimClip->Init(pszSkaPath);
		m_pAnimClip->SetSkeleton(m_pSkeleton.get());
	}

	void AnimClipNode::SetBoneSet(const char* pBoneSetName)
	{
		if (pBoneSetName == NULL)
			return;

		ASSERT(m_pSkeleton);
		if (m_pSkeleton == NULL)
			return;

		BoneSet* pBoneSet = m_pSkeleton->GetBoneSetByName(pBoneSetName);
		if (pBoneSet == NULL)
			return;

		m_pBoneSet = pBoneSet;

	}

	void AnimClipNode::SetAnimationClip(AnimationClip* pAnimClip) 
	{
		if (m_pAnimClip != pAnimClip)
		{
			m_pAnimClip = pAnimClip;
			m_pAnimClip->SetSkeleton(m_pSkeleton.get());
		}
	}

	void AnimClipNode::Serialize(Serializer& sl, const char* pszLable /*= "AnimClipNode" */)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_strClipName,"ClipName");
		sl.Serialize(m_strBoneSetName,"BonsetName");

		sl.EndSection();
	}

	bool AnimClipNode::OnLoadOver()
	{
		if (!m_bClipInit)
		{
			m_pAnimClip->Init(m_strClipName.c_str());
			m_pAnimClip->SetSkeleton(m_pSkeleton.get());
			m_pAnimClip->Instance();
			m_bClipInit = true;
		}

		if (m_pAnimClip && !m_pAnimClip->OnLoadOver())
			return false;
		
		return true;
	}

}
