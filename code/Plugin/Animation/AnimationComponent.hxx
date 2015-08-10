#include "AnimationComponent.h"
#include "AnimationSet.h"

namespace ma
{
	IMPL_OBJECT(AnimationComponent,Component);

	AnimationComponent::AnimationComponent()
	{
		m_nCurAction = 0;
		m_pCurAction = NULL;
		m_pPreAction = NULL;
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
		m_pSkeleton = LoadResource<Skeleton>(pSkePath);

		m_bLoadOver = false;
		IsReady();
	}

	const char* AnimationComponent::GetAnimSetPath() const
	{
		return m_strAnimSetPath.c_str();
	}

	void AnimationComponent::SetAnimSetPath(const char* pAniSetPath)
	{
		m_strAnimSetPath = pAniSetPath ? pAniSetPath : "";

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
	
		if (m_pAnimSet)
		{
			m_pAnimSet->SetSkeleton(m_pSkeleton.get());

			AnimTreeNode* pAnimation = NULL;
			if ( !m_strCurAction.empty() )
			{
				pAnimation = m_pAnimSet->GetSkelAnimByName( m_strCurAction.c_str() );
			}
			else if (m_nCurAction != -1)
			{
				pAnimation = m_pAnimSet->GetSkelAnimByIndex(m_nCurAction);
			}
			ChangeAnimation(pAnimation,m_fFadeTime);
		}

		if (m_pCurAction)
		{
			m_pCurAction->SetSkeletion(m_pSkeleton.get());
			m_pCurAction->Instantiate();
		}

		if (m_pCurAction && !m_pCurAction->IsReady())
		{
			return false;
		}

		m_bLoadOver = true;

		return true;
	}

	void AnimationComponent::Update()
	{
		if ( !IsReady() )
			return;

		if ( m_pCurAction && !m_pCurAction->IsReady() )
			return;

		AdvanceTime( GetTimer()->GetFrameDeltaTime() );

		if (m_pSceneNode->GetLastVisibleFrame() + 1 == GetTimer()->GetFrameCount())
		{
			GetSceneNode()->GetScene()->AddParallelUpdate(this);	
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
		m_pCurAction = NULL;
		m_pPreAction = NULL;
		m_strCurAction = "";
		m_nCurAction = -1; 
	}

	void AnimationComponent::ChangeAnimation(AnimTreeNode* pAnim,float fFadeTime)
	{
		m_pPreAction = m_pCurAction;
		m_pCurAction = pAnim;

		m_fFadeTime = fFadeTime;
		m_fCurFadeTime = fFadeTime;
	}

	void AnimationComponent::PlayAnimation(AnimTreeNode* pSkelAnim,float fFadeTime)
	{
		ChangeAnimation(pSkelAnim,fFadeTime);

		m_bLoadOver = false;
		IsReady();
	}

	void AnimationComponent::PlayAnimation(uint32 actionID,float fFadeTime)
	{
		m_nCurAction = actionID;
		m_fFadeTime = fFadeTime;

		if (m_pAnimSet == NULL)
			return;

		if (actionID < 0 || actionID >= m_pAnimSet->GetSkelAnimNumber())
			return;

		AnimTreeNode* pAnimNode = m_pAnimSet->GetSkelAnimByIndex(actionID);
		ASSERT(pAnimNode);
		if (pAnimNode == NULL)
			return;

		PlayAnimation(pAnimNode, fFadeTime);
	}


	void AnimationComponent::PlayAnimation(const char* pszAnimName,float fFadeTime)
	{
		m_strCurAction = pszAnimName ? pszAnimName : "";

		if (m_pAnimSet == NULL)
			return;

		AnimTreeNode* pAnimNode = m_pAnimSet->GetSkelAnimByName(pszAnimName);
		ASSERT(pAnimNode);
		if (pAnimNode == NULL)
			return;

		PlayAnimation(pAnimNode, fFadeTime);
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

		if (m_pSkeleton == NULL || m_pose == NULL)
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
			m_pPreAction->EvaluateAnimation(&evalContext,fFadeFactor,BLENDMODE_OVERWRITE);
		}

		if (m_pCurAction)
		{
			m_pCurAction->EvaluateAnimation(&evalContext,1.0f - fFadeFactor,BLENDMODE_OVERWRITE);
		}

		m_pose->InitLocalSpace(evalContext.m_arrTSFLS,pRefPose);

		// Do IK

		m_pose->SyncObjectSpace();

		UpdateSkinMatrix();	
	}

	void AnimationComponent::UpdateSkinMatrix()
	{
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
