#include "Animation/AnimationAction.h"

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
		const SkeletonPose* pRefPose = pSkeleton ? pSkeleton->GetResPose() : NULL;
		m_pose = pRefPose ? pRefPose->Clone() : NULL;
	}

	void AnimationPlay::SetAnimationSet(AnimationSet* pAnimationSet)
	{
		m_pAnimSet = pAnimationSet;
	}

	void AnimationPlay::PlayAnimation(AnimationAction* pSkelAnim)
	{
		m_pSkelAnim = pSkelAnim;
	}

	void AnimationPlay::PlayAnimation(const char* pszAnimName)
	{
		if (m_pAnimSet == NULL)
			return;

		m_pSkelAnim = m_pAnimSet->GetAnimationActionByName(pszAnimName);
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

		const SkeletonPose* pRefPose = m_pSkeleton->GetResPose();
		if (pRefPose == NULL)
			return;

		AnimEvalContext evalContext;
		maNodeTransform tsfIdent;
		maTransformSetIdentity(&tsfIdent);
		evalContext.m_arrTSFLS.resize(pRefPose->GetNodeNumber(),tsfIdent);
		evalContext.m_pNodePos = m_pose;
		evalContext.m_refNodePos = pRefPose;

		if (m_pSkelAnim)
		{
			m_pSkelAnim->EvaluateAnimation(&evalContext,fWeight);
		}

// 		for (UINT i = 0; i < m_pose->GetNodeNumber(); ++i)
// 		{
// 			maNodeTransform tsfPS;
// 			maTransfromMul(&tsfPS,&evalContext.m_arrTSFLS[i],&pRefPose->GetTransformPS(i));
// 			m_pose->SetTransformPS(&tsfPS,i);
// 		}
// 
// 		m_pose->SyncObjectSpace();


		UINT nBoneNum = m_pSkeleton->GetBoneNumer();
		for (UINT i = 0; i < nBoneNum; ++i)
		{
			if (m_pose)
			{
				maMatrixFromTransform(&m_arrSkinMatrix[i],& m_pose->GetTransformOS(i));
				D3DXMatrixMultiply(&m_arrSkinMatrix[i],& m_pSkeleton->GetBoneMatrixOSInv(i),&m_arrSkinMatrix[i]);
			}
			else
			{
				D3DXMatrixIdentity(&m_arrSkinMatrix[i]);
			}
		}

	}
}
