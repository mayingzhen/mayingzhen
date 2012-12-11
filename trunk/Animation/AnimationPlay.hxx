#include "Animation/SkeletonAnimation.h"

namespace ma
{
	AnimationPlay::AnimationPlay()
	{
		m_pSkelAnim = NULL;
	}

	AnimationPlay::~AnimationPlay()
	{
	}

	void AnimationPlay::SetSkeleton(Skeleton* pSkeleton)
	{
		m_pSkeleton = pSkeleton;
		const NodePose* pRefPose = pSkeleton ? pSkeleton->GetResPose() : NULL;
		m_pose = pRefPose ? pRefPose->Clone() : NULL;
	}

	void AnimationPlay::PlayAnimation(SkeletonAnimation* pSkelAnim)
	{
		m_pSkelAnim = pSkelAnim;
	}

	void AnimationPlay::AdvanceTime(float fTimeElepse)
	{
		if (m_pSkelAnim)
		{
			m_pSkelAnim->AdvanceTime(fTimeElepse);
		}
	}

	void AnimationPlay::EvaluateAnimation(float fWeight)
	{
		if (m_pSkeleton == NULL)
			return;

		const NodePose* pRefPose = m_pSkeleton->GetResPose();
		if (pRefPose == NULL)
			return;

		AnimEvalContext evalContext;

		maNodeTransform tsfIdent;
		maTransformSetIdentity(&tsfIdent);
		evalContext.m_arrTSFLS.resize(pRefPose->GetNodeNumber(),tsfIdent);

		if (m_pSkelAnim)
		{
			m_pSkelAnim->EvaluateAnimation(&evalContext,fWeight);
		}

		for (UINT i = 0; i < m_pose->GetNodeNumber(); ++i)
		{
			maNodeTransform tsfPS;
			maTransfromMul(&tsfPS,&evalContext.m_arrTSFLS[i],&pRefPose->GetTransformPS(i));
			m_pose->SetTransformPS(&tsfPS,i);
		}

		m_pose->SyncObjectSpace();
	}
}
