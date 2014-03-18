#include "AnimationObject.h"
#include "AnimationSet.h"
#include "Action.h"
#include "AnimationUtil.h"

namespace ma
{
	AnimationObject::AnimationObject(GameObject* pGameObj):
	IAnimationObject(pGameObj)
	{
		m_pCurAction = NULL;
		m_pAnimSet = NULL;
		m_pSkeleton = NULL;
		m_pose = NULL;
		m_arrSkinMatrix = NULL;
	}

	AnimationObject::~AnimationObject()
	{
		SAFE_DELETE_ARRAY(m_arrSkinMatrix);
	}

	void AnimationObject::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		SAFE_DELETE(m_pose);
		SAFE_DELETE(m_pAnimSet);

		m_pSkeleton = LoadResourceSync<Skeleton>(pszSkeletonPath);
		const SkeletonPose* pRefPose = m_pSkeleton ? m_pSkeleton->GetResPose() : NULL;
		m_pose = pRefPose ? pRefPose->Clone() : NULL;
		UINT nBone = m_pSkeleton->GetBoneNumer();
		m_arrSkinMatrix = new Matrix4x4[nBone];
		for (UINT i = 0; i < nBone; ++i)
		{
			MatrixIdentity(&m_arrSkinMatrix[i]);
		}

		m_pAnimSetData = LoadResourceSync<AnimationSetData>(pszAniSetPath);
		m_pAnimSet = new AnimationSet(this,m_pAnimSetData);
	}

	void AnimationObject::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		std::string strAnimSetPath = m_pAnimSetData ? m_pAnimSetData->GetResPath() : "";
		std::string strSkeletonPath = m_pSkeleton ? m_pSkeleton->GetResPath() : "";

		sl.Serialize(strAnimSetPath);
		sl.Serialize(strSkeletonPath);

		if ( sl.IsReading() )
		{
			Load(strAnimSetPath.c_str(),strSkeletonPath.c_str());
		}

		sl.EndSection();
	}

// 	void AnimationObject::Update()
// 	{
// 		if (m_pSkeleton->GetResState() == ResLoaded && m_pose == NULL)
// 		{
// 			const SkeletonPose* pRefPose = m_pSkeleton ? m_pSkeleton->GetResPose() : NULL;
// 			m_pose = pRefPose ? pRefPose->Clone() : NULL;
// 
// 			UINT nBone = m_pSkeleton->GetBoneNumer();
// 			m_arrSkinMatrix = new Matrix4x4[nBone];
// 			for (UINT i = 0; i < nBone; ++i)
// 			{
// 				MatrixIdentity(&m_arrSkinMatrix[i]);
// 			}
// 		}
// 
// 		if (m_pAnimSetData->GetResState() == ResLoaded && m_pAnimSet == NULL)
// 		{
// 			m_pAnimSet = new AnimationSet(this,m_pAnimSetData);
// 		}
// 	}

	void AnimationObject::PlayAnimation(Action* pSkelAnim)
	{
		m_pCurAction = pSkelAnim;
	}

	void AnimationObject::PlayAnimation(ActionID actionID)
	{
		if (m_pAnimSet == NULL)
			return;

		if (actionID < 0 || actionID >= m_pAnimSet->GetActionNumber())
			return;

		PlayAnimation( (Action*)m_pAnimSet->GetActionByIndex(actionID) );
	}


	void AnimationObject::PlayAnimation(const char* pszAnimName)
	{
		if (m_pAnimSet == NULL)
			return;

		PlayAnimation( (Action*)m_pAnimSet->GetActionByName(pszAnimName) );
	}

	void AnimationObject::AdvanceTime(float fTimeElepse)
	{
		if (m_pCurAction == NULL)
			return;

		m_pCurAction->AdvanceTime(fTimeElepse);
	}

	void AnimationObject::EvaluateAnimation(float fWeight)
	{
		profile_code();

		if (m_pSkeleton == NULL  || m_pCurAction == NULL)
			return;

		const SkeletonPose* pRefPose = m_pSkeleton->GetResPose();
		if (pRefPose == NULL)
			return;

		AnimEvalContext evalContext;
		NodeTransform tsfIdent;
		TransformSetIdentity(&tsfIdent);
		evalContext.m_arrTSFLS.resize(pRefPose->GetNodeNumber(),tsfIdent);
		evalContext.m_pNodePos = m_pose;
		evalContext.m_refNodePos = pRefPose;

		if (m_pCurAction)
		{
			m_pCurAction->EvaluateAnimation(&evalContext,fWeight);
		}

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

		UINT nMeshComp = m_pGameObject->GetTypeComponentNumber<MeshComponent>();
		for (UINT i = 0; i < nMeshComp; ++i)
		{
			MeshComponentPtr pMeshComp = m_pGameObject->GetTypeComponentByIndex<MeshComponent>(i);
			ASSERT(pMeshComp);
			if (pMeshComp == NULL)
				continue;

			pMeshComp->SetSkinMatrix(m_arrSkinMatrix,nBoneNum);
		}
	}

	void AnimationObject::SetFrame(float fFrame)
	{
		if (m_pCurAction)
		{
			m_pCurAction->SetFrame(fFrame);
		}
	}

}
