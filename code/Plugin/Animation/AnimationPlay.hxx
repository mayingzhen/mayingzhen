#include "AnimationPlay.h"
#include "AnimationSet.h"
#include "Action.h"
#include "AnimationUtil.h"

namespace ma
{
	AnimationPlay::AnimationPlay()
	{
		m_pSkelAnim = NULL;
		m_pAnimSet = NULL;
		m_pSkeleton = NULL;
		m_pose = NULL;

		for (UINT i = 0; i < 256; ++i)
		{
			MatrixIdentity(&m_arrSkinMatrix[i]);
		}
	}

	AnimationPlay::~AnimationPlay()
	{
	}

	ISkeleton*		AnimationPlay::CreateSkeleton(const char* pSkePath)
	{
		ASSERT(pSkePath);
		if (pSkePath == NULL)
			return NULL;

		SkeletonData* pSkeData = static_cast<SkeletonData*>(ResourceManager::DeclareResource(pSkePath));
		pSkeData->Load();
		pSkeData->LoadImp(pSkeData->GetDataStream());

		m_pSkeleton = new Skeleton();
		m_pSkeleton->InitWithData(pSkeData);

		const SkeletonPose* pRefPose = m_pSkeleton ? m_pSkeleton->GetResPose() : NULL;
		m_pose = pRefPose ? pRefPose->Clone() : NULL;

		return m_pSkeleton;
	}

	IAnimationSet*	AnimationPlay::CreateAnimSet(const char* pAnimSetPath)
	{
		m_pAnimSet = new AnimationSet(this);

		return m_pAnimSet;
	}

	void AnimationPlay::PlayAnimation(Action* pSkelAnim)
	{
		m_pSkelAnim = pSkelAnim;
	}

	void AnimationPlay::PlayAnimation(ActionID actionID)
	{
		if (m_pAnimSet == NULL)
			return;

		if (actionID < 0 || actionID >= m_pAnimSet->GetActionNumber())
			return;

		m_pSkelAnim = m_pAnimSet->GetActionByIndex(actionID);
	}


	void AnimationPlay::PlayAnimation(const char* pszAnimName)
	{
		if (m_pAnimSet == NULL)
			return;

		m_pSkelAnim = m_pAnimSet->GetActionByName(pszAnimName);
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
		NodeTransform tsfIdent;
		//TransformSetIdentity(&tsfIdent);
		memset(&tsfIdent,0,sizeof(NodeTransform));
		evalContext.m_arrTSFLS.resize(pRefPose->GetNodeNumber(),tsfIdent);
		evalContext.m_pNodePos = m_pose;
		evalContext.m_refNodePos = pRefPose;

		if (m_pSkelAnim)
		{
			m_pSkelAnim->EvaluateAnimation(&evalContext,fWeight);
		}

// 		for (UINT i = 0; i < m_pose->GetNodeNumber(); ++i)
// 		{
// 			NodeTransform tsfPS;
// 			maTransformMul(&tsfPS,&evalContext.m_arrTSFLS[i],&pRefPose->GetTransformPS(i));
// 			m_pose->SetTransformPS(&tsfPS,i);
// 		}
// 
// 		m_pose->SyncObjectSpace();


		UINT nBoneNum = m_pSkeleton->GetBoneNumer();
		for (UINT i = 0; i < nBoneNum; ++i)
		{
			if (m_pose)
			{
				MatrixFromTransform(&m_arrSkinMatrix[i],& m_pose->GetTransformOS(i));
				MatrixMultiply(&m_arrSkinMatrix[i],& m_pSkeleton->GetBoneMatrixOSInv(i),&m_arrSkinMatrix[i]);
			}
			else
			{
				MatrixIdentity(&m_arrSkinMatrix[i]);
			}
		}

	}
}
