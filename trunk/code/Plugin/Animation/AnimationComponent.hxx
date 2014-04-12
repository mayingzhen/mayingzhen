#include "AnimationComponent.h"
#include "AnimationSet.h"
#include "SkelAnimtion.h"
#include "AnimationUtil.h"

namespace ma
{
	IMPL_OBJECT(AnimationComponent,Component);

	AnimationComponent::AnimationComponent(GameObject* pGameObj):
	Component(pGameObj)
	{
		m_pCurAction = NULL;
		m_pAnimSet = NULL;
		m_pSkeleton = NULL;
		m_pose = NULL;
		m_arrSkinMatrix = NULL;
	}

	AnimationComponent::~AnimationComponent()
	{
		SAFE_DELETE_ARRAY(m_arrSkinMatrix);
	}

	void AnimationComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
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
		m_pAnimSet = new AnimationSet(m_pSkeleton,m_pAnimSetData);
	}

	void AnimationComponent::Serialize(Serializer& sl, const char* pszLable)
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

// 	void AnimationComponent::Update()
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

	void AnimationComponent::Update()
	{
		AdvanceTime( GetTimer()->GetFrameDeltaTime() );

		if (1/*m_pGameObject->V*/)
		{
			EvaluateAnimation(1.0f);
		}
	}

	void AnimationComponent::PlayAnimation(SkelAnimtion* pSkelAnim)
	{
		m_pCurAction = pSkelAnim;
	}

	void AnimationComponent::PlayAnimation(ActionID actionID)
	{
		if (m_pAnimSet == NULL)
			return;

		if (actionID < 0 || actionID >= m_pAnimSet->GetSkelAnimNumber())
			return;

		PlayAnimation( (SkelAnimtion*)m_pAnimSet->GetSkelAnimByIndex(actionID) );
	}


	void AnimationComponent::PlayAnimation(const char* pszAnimName)
	{
		if (m_pAnimSet == NULL)
			return;

		PlayAnimation( (SkelAnimtion*)m_pAnimSet->GetSkelAnimByName(pszAnimName) );
	}

	void AnimationComponent::AdvanceTime(float fTimeElepse)
	{
		if (m_pCurAction == NULL)
			return;

		m_pCurAction->AdvanceTime(fTimeElepse);
	}

	void AnimationComponent::EvaluateAnimation(float fWeight)
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

		std::vector<MeshComponent*> arrMeshComp;
		m_pGameObject->GetTypeComponent<MeshComponent>(arrMeshComp);
		for (UINT i = 0; i < arrMeshComp.size(); ++i)
		{
			MeshComponent* pMeshComp = arrMeshComp[i];
			ASSERT(pMeshComp);
			if (pMeshComp == NULL)
				continue;

			pMeshComp->SetSkinMatrix(m_arrSkinMatrix,nBoneNum);
		}
	}

	void AnimationComponent::SetFrame(float fFrame)
	{
		if (m_pCurAction)
		{
			m_pCurAction->SetFrame(fFrame);
		}
	}

}
