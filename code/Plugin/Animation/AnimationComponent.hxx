#include "AnimationComponent.h"
#include "AnimationSet.h"

namespace ma
{
	AnimationComponent::AnimationComponent()
	{
		m_nCurAction = -1;
		m_pAnimation = NULL;
		m_pPreAnimation = NULL;
		m_pAnimSet = NULL;
		m_pSkeleton = NULL;
		m_pose = NULL;
		m_arrSkinMatrix = NULL;
		
		m_bLoadOver = false;
	}

	AnimationComponent::~AnimationComponent()
	{
		SAFE_DELETE_ARRAY(m_arrSkinMatrix);
		SAFE_DELETE(m_pose);
	}

	void AnimationComponent::RegisterAttribute()
	{
		REGISTER_OBJECT(AnimationComponent,CreateAnimationComponent)

		ACCESSOR_ATTRIBUTE(AnimationComponent, "AnimSetPath", GetAnimSetPath, SetAnimSetPath, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(AnimationComponent, "SkeletonPath", GetSkeletonPath, SetSkeletonPath, const char*, NULL, AM_DEFAULT);
	}

	const char* AnimationComponent::GetSkeletonPath() const
	{
		return m_pSkeleton->GetResPath();
	}

	void AnimationComponent::SetSkeletonPath(const char* pSkePath)
	{
		SAFE_DELETE(m_pose);
		m_pSkeleton = CreateSkeleton(pSkePath);

		m_bLoadOver = false;
		IsReady();
	}

	const char* AnimationComponent::GetAnimSetPath() const
	{
		if (m_pAnimSet == NULL || m_pAnimSet->GetXMLFile() == NULL)
			return NULL;

		return m_pAnimSet->GetXMLFile()->GetResPath();
	}

	void AnimationComponent::SetAnimSetPath(const char* pAniSetPath)
	{
		m_pAnimSet = CreateAnimationSet(pAniSetPath);
		
		m_bLoadOver = false;
		IsReady();
	}

	void AnimationComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		SetSkeletonPath(pszSkeletonPath);

		SetAnimSetPath(pszAniSetPath);
	}

	bool AnimationComponent::IsReady()
	{
		if (m_bLoadOver)
			return true;

		if ( m_pSkeleton == NULL || !m_pSkeleton->IsReady() )
			return false;

		if ( m_pAnimSet && !m_pAnimSet->IsReady() )
			return false;

		if (m_pose == NULL)
		{
			const SkeletonPose* pRefPose = m_pSkeleton ? m_pSkeleton->GetResPose() : NULL;
			m_pose = pRefPose ? pRefPose->Clone() : NULL;
			UINT nBone = m_pSkeleton->GetBoneNumer();
			m_arrSkinMatrix = new Matrix3x4[nBone];
			for (UINT i = 0; i < nBone; ++i)
			{
				m_arrSkinMatrix[i] = Matrix4::IDENTITY;
			}
		}
	
		if (m_pAnimSet && m_nCurAction != -1)
		{
			ChangeAnimation( m_pAnimSet->GetAnimationByAnimID(m_nCurAction) );
		}

		if (m_pAnimation == NULL)
			return false;

		m_pAnimation->Instantiate(m_pSkeleton.get());

		if (!m_pAnimation->IsReady())
		{
			return false;
		}

		if (m_pPreAnimation && !m_pPreAnimation->IsReady())
		{
			return false;
		}

		m_bLoadOver = true;

		return true;
	}

	void AnimationComponent::Update()
	{
		AdvanceTime( GetTimer()->GetFrameDeltaTime() );

		// 动作回调完以后，动作有可能被改变了
		IsReady();

		if ( GetTimer()->GetFrameCount() - m_pSceneNode->GetLastVisibleFrame() > 1 )
			return;

		if (GetJobScheduler()->GetNumThreads() > 0)
		{
			GetSceneNode()->GetScene()->AddParallelUpdate(this);	
		}
		else
		{
			ParallelUpdate();
			EndParallelUpdate();
		}
	}

	void AnimationComponent::ParallelUpdate()
	{
		EvaluateAnimation();
	}

	void AnimationComponent::EndParallelUpdate()
	{

	}

	void AnimationComponent::Stop()
	{
		m_pAnimation = NULL;
		m_pPreAnimation = NULL;
		m_nCurAction = -1; 
	}

	void AnimationComponent::ChangeAnimation(AnimTreeNode* pAnim)
	{
		m_pPreAnimation = m_pAnimation;
		m_pAnimation = pAnim;

		if (pAnim)
		{
			pAnim->Play();
			m_fCurFadeTime = pAnim->GetFadeTime();
		}
	}

	void AnimationComponent::PlayAnimation(AnimTreeNode* pSkelAnim)
	{
		AutoLock lock(m_csParallelUpdate);

		if (m_pAnimation == pSkelAnim)
			return;
	
		ChangeAnimation(pSkelAnim);

		m_bLoadOver = false;
		IsReady();
	}

	void AnimationComponent::PlayAnimation(uint32 actionID)
	{
		AutoLock lock(m_csParallelUpdate);

		ASSERT(actionID != -1);
		if (actionID == -1)
			return;

		m_nCurAction = actionID;

		m_bLoadOver = false;
		IsReady();
	}


	void AnimationComponent::PlayAnimation(const char* pszAnimName)
	{
		PlayAnimation( AnimTreeNode::AnimNameToID(pszAnimName) );
	}


	void AnimationComponent::AdvanceTime(float fTimeElepse)
	{
		m_fCurFadeTime -= fTimeElepse;
		if (m_fCurFadeTime <= 0)
		{
			m_fCurFadeTime = 0;
			m_pPreAnimation = NULL;
		}

		if (m_pAnimation)
		{
			m_pAnimation->AdvanceTime(fTimeElepse);
		}
	}

	void AnimationComponent::EvaluateAnimation()
	{
		profile_code();

		if (m_pSkeleton == NULL || m_pose == NULL)
			return;

		const SkeletonPose* pRefPose = m_pSkeleton->GetResPose();
		if (pRefPose == NULL)
			return;

		AnimEvalContext evalContext;
		Transform tsfInit;
		evalContext.m_arrTSFPS.resize( pRefPose->GetNodeNumber() , tsfInit);
		evalContext.m_arrFirst.resize(pRefPose->GetNodeNumber() , true);
		evalContext.m_pNodePos = m_pose;
		evalContext.m_refNodePos = pRefPose;

		float fFadeTime = m_pAnimation && m_pPreAnimation ? m_pAnimation->GetFadeTime() : 0; 
		float fFadeFactor = fFadeTime <= 0 ? 0.0f : (m_fCurFadeTime / fFadeTime);

		if (m_pPreAnimation && fFadeFactor > 0)
		{
			m_pPreAnimation->EvaluateAnimation(&evalContext,fFadeFactor);
		}
		
		if (m_pAnimation)
		{
			m_pAnimation->EvaluateAnimation(&evalContext,1.0f - fFadeFactor);
		}

		m_pose->InitLocalSpace(evalContext.m_arrTSFPS,pRefPose);

		////////////// Do IK
		if (m_pPreAnimation && fFadeFactor > 0)
		{
			m_pPreAnimation->ProcessPoseModifier(m_pose,fFadeFactor);
		}

		if (m_pAnimation)
		{
			m_pAnimation->ProcessPoseModifier(m_pose,1.0f - fFadeFactor);
		}
		////////////////

		m_pose->SyncObjectSpace();

		UpdateSkinMatrix();	
	}

	void AnimationComponent::UpdateSkinMatrix()
	{
		if (m_pSkeleton == NULL)
			return;

		UINT nBoneNum = m_pSkeleton->GetBoneNumer();
		for (UINT i = 0; i < nBoneNum; ++i)
		{
			Matrix4 matSkin;
			MatrixFromTransform(&matSkin,& m_pose->GetTransformOS(i));
			matSkin = matSkin * m_pSkeleton->GetBoneMatrixOSInv(i);
			m_arrSkinMatrix[i] = matSkin;
		}

		std::vector<SkinMeshComponent*> arrMeshComp;
		m_pSceneNode->GetTypeComponent<SkinMeshComponent>(arrMeshComp);
		for (UINT i = 0; i < arrMeshComp.size(); ++i)
		{
			SkinMeshComponent* pMeshComp = arrMeshComp[i];
			ASSERT(pMeshComp);
			if (pMeshComp == NULL)
				continue;

			pMeshComp->SetSkinMatrix(m_arrSkinMatrix,nBoneNum);
		}
	}

	void AnimationComponent::DebugRender(bool bDrawBoneName/* = false*/)
	{
		if (m_pose)
		{
			m_pose->DebugRender(m_pSceneNode->GetMatrixWS(),bDrawBoneName,m_pSkeleton.get());
		}
	}

	RefPtr<AnimationComponent> CreateAnimationComponent()
	{
		return new AnimationComponent;
	}

}
