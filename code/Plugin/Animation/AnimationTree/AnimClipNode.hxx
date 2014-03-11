#include "Animation/AnimationTree/AnimClipNode.h"


namespace ma
{
	IMPL_OBJECT(AnimClipNode,IAnimClipNode)

	AnimClipNode::AnimClipNode(Skeleton* pSkeleton)
	{
		m_pBoneSet = NULL;
		m_pAnimClip = NULL;

		m_pSkeleton = pSkeleton;
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

	void AnimClipNode::SetAnimationClip(const char* pszSkaPath)
	{
		ASSERT(m_pAnimClip == NULL);
		if (m_pAnimClip)
		{
			SAFE_DELETE(m_pAnimClip);
		}

		ref_ptr<Animation> pAnim = LoadResourceSync<Animation>(pszSkaPath);
		m_pAnimClip = new AnimationClip(pAnim,m_pSkeleton);
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

// 	void AnimClipNode::Serialize(Serializer& sl, const char* pszLable /*= "AnimClipNode" */)
// 	{
// 		sl.BeginSection(pszLable);
// 
// 		if (sl.IsReading())
// 		{
// 			std::string strAnimClipName;
// 			std::string strBonSetName;
// 
// 			SAFE_DELETE(m_pBoneSet);
// 			SAFE_DELETE(m_pAnimClip);
// 
// 			sl.Serialize(strAnimClipName,"ClipName");
// 			sl.Serialize(strBonSetName,"BonsetName");
// 
// 			SetAnimationClip(strAnimClipName.c_str());
// 			SetBoneSet(strBonSetName.c_str());
// 		}
// 		else
// 		{
// 			std::string strAnimClipName = m_pAnimClip->GetAnimation()->GetResPath();
// 			std::string strBonSetName = m_pBoneSet ? m_pBoneSet->GetBoneSetName() : "";
// 
// 			sl.Serialize(strAnimClipName,"ClipName");
// 			sl.Serialize(strBonSetName,"BonsetName");
// 		}
// 
// 		sl.EndSection();
// 	}

// 	void AnimClipNode::SetSkeleton(Skeleton* pSkeleton) 
// 	{
// 		if (m_pSkeleton != pSkeleton)
// 		{
// 			m_pSkeleton = pSkeleton;
// 			m_pAnimClip->SetSkeleton(pSkeleton);
// 		}
// 	}

	void AnimClipNode::SetAnimationClip(AnimationClip* pAnimClip) 
	{
		if (m_pAnimClip != pAnimClip)
		{
			m_pAnimClip = pAnimClip;
			m_pAnimClip->SetSkeleton(m_pSkeleton);
		}
	}
}
