#include "AnimationComponent.h"
#include "AnimationSet.h"
#include "SkelAnimtion.h"
#include "AnimationUtil.h"

namespace ma
{
	IMPL_OBJECT(AnimationComponent,Component);

	AnimationComponent::AnimationComponent(SceneNode* pGameObj):
	Component(pGameObj)
	{
		m_pCurAction = NULL;
		m_pPreAction = NULL;
		m_pAnimSet = NULL;
		m_pSkeleton = NULL;
		m_pose = NULL;
		m_arrSkinMatrix = NULL;

		m_pElAniSetLoaded = new ELAnimationComponent(this,&AnimationComponent::CreateAniSet);
		m_pElSkeletonLoaded = new ELAnimationComponent(this,&AnimationComponent::CreateSkeletonPose);
	}

	AnimationComponent::~AnimationComponent()
	{
		SAFE_DELETE_ARRAY(m_arrSkinMatrix);
	}

	void AnimationComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		SAFE_DELETE(m_pose);
		SAFE_DELETE(m_pAnimSet);

		m_pSkeleton = LoadResourceSync<Skeleton>(pszSkeletonPath/*,m_pElSkeletonLoaded.get()*/);
		CreateSkeletonPose(NULL,NULL);	

		m_pAnimSetData = LoadResourceSync<AnimationSetData>(pszAniSetPath/*,m_pElAniSetLoaded.get()*/);	
		CreateAniSet(NULL,NULL);
	}

	void AnimationComponent::CreateAniSet(void* parm1,void* parm2)
	{
		ASSERT(m_pSkeleton);
		ASSERT(m_pAnimSetData);

		m_pAnimSet = new AnimationSet(m_pSkeleton,m_pAnimSetData);
	}

	void AnimationComponent::CreateSkeletonPose(void* parm1,void* parm2)
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

	void AnimationComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		std::string strAnimSetPath = m_pAnimSetData ? m_pAnimSetData->GetResPath() : "";
		std::string strSkeletonPath = m_pSkeleton ? m_pSkeleton->GetResPath() : "";

		sl.Serialize(strAnimSetPath,"AnimSetPath");
		sl.Serialize(strSkeletonPath,"SkeletonPath");

		if ( sl.IsReading() )
		{
			Load(strAnimSetPath.c_str(),strSkeletonPath.c_str());
		}

		sl.EndSection();
	}

	void AnimationComponent::Update()
	{
		AdvanceTime( GetTimer()->GetFrameDeltaTime() );

		if (m_pSceneNode->GetLastVisibleFrame() == GetTimer()->GetFrameCount())
		{
			EvaluateAnimation();
		}
	}

	void AnimationComponent::Stop()
	{
		m_pCurAction = NULL;
		m_pPreAction = NULL;
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

		if (m_pSkeleton == NULL  || m_pCurAction == NULL)
			return;

		const SkeletonPose* pRefPose = m_pSkeleton->GetResPose();
		if (pRefPose == NULL)
			return;

		AnimEvalContext evalContext;
		evalContext.m_arrTSFLS.resize( pRefPose->GetNodeNumber() );
		evalContext.m_pNodePos = m_pose;
		evalContext.m_refNodePos = pRefPose;

		float fBlendFactor = m_fFadeTime <= 0 ? 1.0f : 1.0f - (m_fCurFadeTime / m_fFadeTime);

		if (m_pPreAction && fBlendFactor < 1.0f)
		{
			m_pPreAction->EvaluateAnimation(&evalContext,fBlendFactor);
		}

		if (m_pCurAction)
		{
			m_pCurAction->EvaluateAnimation(&evalContext,fBlendFactor);
		}

		UpdateSkinMatrix();	
	}

	void AnimationComponent::UpdateSkinMatrix()
	{
		UINT nBoneNum = m_pSkeleton->GetBoneNumer();
		for (UINT i = 0; i < nBoneNum; ++i)
		{
			if (m_pose)
			{
				MatrixFromTransform(&m_arrSkinMatrix[i],& m_pose->GetTransformOS(i));
				m_arrSkinMatrix[i] = m_arrSkinMatrix[i] * m_pSkeleton->GetBoneMatrixOSInv(i);
			}
			else
			{
				m_arrSkinMatrix[i] = Matrix4::IDENTITY;
			}
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
			m_pose->DebugRender(m_pSceneNode->GetWorldMatrix(),bDrawBoneName,m_pSkeleton.get());
		}
	}

}
