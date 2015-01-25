#include "AnimationComponent.h"
#include "AnimationSet.h"
#include "SkelAnimtion.h"

namespace ma
{
	IMPL_OBJECT(AnimationComponent,Component);

	AnimationComponent::AnimationComponent()
	{
		m_pCurAction = NULL;
		m_pPreAction = NULL;
		m_pAnimSet = NULL;
		m_pSkeleton = NULL;
		m_pose = NULL;
		m_arrSkinMatrix = NULL;
		
		m_bLoadOver = false;
	
		m_nCurAction = -1;
	}

	AnimationComponent::~AnimationComponent()
	{
		SAFE_DELETE_ARRAY(m_arrSkinMatrix);
	}

	void AnimationComponent::RegisterAttribute()
	{
		ACCESSOR_ATTRIBUTE(AnimationComponent, "AnimSetPath", GetAnimSetPath, SetAnimSetPath, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimationComponent, "SkeletonPath", GetSkeletonPath, SetSkeletonPath, const char*, NULL, AM_DEFAULT);
	}

	const char* AnimationComponent::GetSkeletonPath() const
	{
		return m_pSkeleton->GetResPath();
	}

	void AnimationComponent::SetSkeletonPath(const char* pSkePath)
	{
		m_pSkeleton = LoadResource<Skeleton>(pSkePath);
	}

	const char* AnimationComponent::GetAnimSetPath() const
	{
		return m_strAnimSetPath.c_str();
	}

	void AnimationComponent::SetAnimSetPath(const char* pAniSetPath)
	{
		m_strAnimSetPath = pAniSetPath ? pAniSetPath : "";

		m_pAnimSet = new AnimationSet();
		m_pAnimSet->LoadFromXML(pAniSetPath);
	}

	void AnimationComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		SAFE_DELETE(m_pose);

		SetSkeletonPath(pszSkeletonPath);

		SetAnimSetPath(pszAniSetPath);

		OnLoadOver();
	}

	void AnimationComponent::CreateSkeletonPose()
	{
		ASSERT(m_pSkeleton);

		const SkeletonPose* pRefPose = m_pSkeleton ? m_pSkeleton->GetResPose() : NULL;
		m_pose = pRefPose ? pRefPose->Clone() : NULL;
		UINT nBone = m_pSkeleton->GetBoneNumer();
		m_arrSkinMatrix = new Matrix4[nBone];
		for (UINT i = 0; i < nBone; ++i)
		{
			m_arrSkinMatrix[i] = Matrix4::IDENTITY;
		}
	}

	bool AnimationComponent::OnLoadOver()
	{
		if (m_bLoadOver)
			return true;

		ASSERT(m_pSkeleton && m_pAnimSet);

		if ( m_pSkeleton && !m_pSkeleton->OnLoadOver() )
			return false;

		if ( m_pAnimSet && !m_pAnimSet->OnLoadOver() )
			return false;

		CreateSkeletonPose();

		m_pAnimSet->SetSkeleton(m_pSkeleton.get());

		if (m_pCurAction == NULL && m_strCurAction != "")
			PlayAnimation(m_strCurAction.c_str());
		else if (m_pCurAction == NULL && m_nCurAction >= 0)
			PlayAnimation(m_nCurAction);
		
		m_bLoadOver = true;

		return true;
	}

	void AnimationComponent::Update()
	{
		if ( !OnLoadOver() )
			return;

		if ( m_pCurAction && !m_pCurAction->OnLoadOver() )
			return;

		AdvanceTime( GetTimer()->GetFrameDeltaTime() );

		if (m_pSceneNode->GetLastVisibleFrame() + 1 == GetTimer()->GetFrameCount())
		{
			EvaluateAnimation();
		}
	}

	void AnimationComponent::Stop()
	{
		m_pCurAction = NULL;
		m_pPreAction = NULL;
		m_strCurAction = "";
		m_nCurAction = -1; 
	}

	void AnimationComponent::PlayAnimation(SkelAnimtion* pSkelAnim,float fFadeTime)
	{
		m_pPreAction = m_pCurAction;
		m_pCurAction = pSkelAnim;

		m_fFadeTime = fFadeTime;
		m_fCurFadeTime = fFadeTime;
	}

	void AnimationComponent::PlayAnimation(ActionID actionID)
	{
		m_nCurAction = actionID;

		if (m_pAnimSet == NULL)
			return;

		if (actionID < 0 || actionID >= m_pAnimSet->GetSkelAnimNumber())
			return;

		PlayAnimation( (SkelAnimtion*)m_pAnimSet->GetSkelAnimByIndex(actionID) );
	}


	void AnimationComponent::PlayAnimation(const char* pszAnimName)
	{
		m_strCurAction = pszAnimName ? pszAnimName : "";

		if (m_pAnimSet == NULL)
			return;

		PlayAnimation( (SkelAnimtion*)m_pAnimSet->GetSkelAnimByName(pszAnimName) );
	}

	void AnimationComponent::AdvanceTime(float fTimeElepse)
	{
		m_fCurFadeTime -= fTimeElepse;

		if (m_fCurFadeTime <= 0)
		{
			m_fCurFadeTime = 0;
			m_pPreAction = NULL;
		}

		if (m_pCurAction)
		{
			m_pCurAction->AdvanceTime(fTimeElepse);
		}

		if (m_pPreAction)
		{
			m_pPreAction->AdvanceTime(fTimeElepse);
		}
	}

	void AnimationComponent::EvaluateAnimation()
	{
		profile_code();

		if (m_pSkeleton == NULL  || m_pCurAction == NULL || m_pose == NULL)
			return;

		const SkeletonPose* pRefPose = m_pSkeleton->GetResPose();
		if (pRefPose == NULL)
			return;

		AnimEvalContext evalContext;
		evalContext.m_arrTSFLS.resize( pRefPose->GetNodeNumber() );
		evalContext.m_pNodePos = m_pose;
		evalContext.m_refNodePos = pRefPose;

		float fFadeFactor = m_fFadeTime <= 0 ? 0.0f : (m_fCurFadeTime / m_fFadeTime);

		if (m_pPreAction && fFadeFactor > 0)
		{
			m_pPreAction->EvaluateAnimation(&evalContext,fFadeFactor);
		}

		if (m_pCurAction)
		{
			m_pCurAction->EvaluateAnimation(&evalContext,1.0f - fFadeFactor);
		}

		UpdateSkinMatrix();	
	}

	void AnimationComponent::UpdateSkinMatrix()
	{
		UINT nBoneNum = m_pSkeleton->GetBoneNumer();
		for (UINT i = 0; i < nBoneNum; ++i)
		{
			MatrixFromTransform(&m_arrSkinMatrix[i],& m_pose->GetTransformOS(i));
			m_arrSkinMatrix[i] = m_arrSkinMatrix[i] * m_pSkeleton->GetBoneMatrixOSInv(i);
	
			//m_arrSkinMatrix[i] = Matrix4::IDENTITY;
		}

		std::vector<MeshComponent*> arrMeshComp;
		m_pSceneNode->GetTypeComponent<MeshComponent>(arrMeshComp);
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

	void AnimationComponent::DebugRender(bool bDrawBoneName/* = false*/)
	{
		if (m_pose)
		{
			m_pose->DebugRender(m_pSceneNode->GetMatrixWS(),bDrawBoneName,m_pSkeleton.get());
		}
	}

}
