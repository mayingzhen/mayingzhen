#include "AnimationComponent.h"
#include "AnimationSet.h"

namespace ma
{

	AnimationComponent::AnimationComponent()
	{
		m_pAnimSet = NULL;
		m_pSkeleton = NULL;
		m_pose = NULL;
		m_arrSkinMatrix = NULL;
		
		m_bLoadOver = false;
	}

	AnimationComponent::~AnimationComponent()
	{
		SAFE_DELETE_ARRAY(m_arrSkinMatrix);
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

	void AnimationComponent::SetAnimationSet(AnimationSet* pAnimSet)
	{
		m_pAnimSet = pAnimSet;

		m_bLoadOver = false;
		IsReady();
	}

	void AnimationComponent::SetAnimSetPath(const char* pAniSetPath)
	{
		SetAnimationSet( CreateAnimationSet(pAniSetPath).get() );
	}

	void AnimationComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		SetSkeletonPath(pszSkeletonPath);

		SetAnimSetPath(pszAniSetPath);
	}

	void AnimationComponent::SetLayerName(uint32 nLayer,const char* pszName)
	{
		ASSERT(pszName);
		if (pszName == NULL)
			return;

		if (nLayer >= m_arrLayerInfo.size())
		{
			m_arrLayerInfo.resize(nLayer + 1);
		}

		ASSERT(m_arrLayerInfo[nLayer].m_strLayerName.empty());
		m_arrLayerInfo[nLayer].m_strLayerName = pszName;

		m_bLoadOver = false;
		IsReady();
	}


	void AnimationComponent::SetGoalWorldSpace(Vector3 vGoalWS)
	{
		if (m_pSceneNode == NULL)
			return;

 		Matrix4 matWSInv = m_pSceneNode->GetMatrixWS().inverse();
		Vector3 vGoalOS = matWSInv * vGoalWS;

		for (UINT32 i = 0; i < m_arrLayerInfo.size(); ++i)
		{
			if (m_arrLayerInfo[i].m_pAnimation)
			{
				m_arrLayerInfo[i].m_pAnimation->SetGoalObjectSpace(vGoalOS);
			}
		}
	}

	bool AnimationComponent::IsReady()
	{
		if (m_bLoadOver)
			return true;

		if ( m_pSkeleton == NULL || !m_pSkeleton->IsReady() )
			return false;

		if ( m_pAnimSet == NULL || !m_pAnimSet->IsReady() )
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

		for (UINT32 i = 0; i < m_arrLayerInfo.size(); ++i)
		{
			if (m_arrLayerInfo[i].m_nAnimID != -1 && m_pAnimSet)
			{
				AnimTreeNode* pAnimation = m_pAnimSet->GetAnimationByAnimID(m_arrLayerInfo[i].m_nAnimID);

				PlayAnimation(pAnimation,i);

				m_arrLayerInfo[i].m_nAnimID = -1; 
			}

			if (m_arrLayerInfo[i].m_pAnimation)
			{
				m_arrLayerInfo[i].m_pAnimation->Instantiate(m_pSkeleton.get());

				if (!m_arrLayerInfo[i].m_pAnimation->IsReady())
				{
					return false;
				}
			}

			if (m_arrLayerInfo[i].m_pPreAnimation && !m_arrLayerInfo[i].m_pPreAnimation->IsReady())
			{
				return false;
			}
		}

		
		m_bLoadOver = true;

		return true;
	}

	void AnimationComponent::PlayAnimation(AnimTreeNode* pAnimation,uint32 nLayerID)
	{	
		AutoLock lock(m_csParallelUpdate);

		if (m_arrLayerInfo[nLayerID].m_pPreAnimation == NULL)
		{
			m_arrLayerInfo[nLayerID].m_pPreAnimation = m_arrLayerInfo[nLayerID].m_pAnimation;
		}
		else
		{
			// 上一个动作还没有过度完成
			ASSERT(m_arrLayerInfo[nLayerID].m_pAnimation);

			RefPtr<AnimBlendNode> pAnimBlend = new AnimBlendNode();
			pAnimBlend->SetSrcAnimNode(m_arrLayerInfo[nLayerID].m_pPreAnimation.get());
			pAnimBlend->SetDestAnimNode(m_arrLayerInfo[nLayerID].m_pAnimation.get());
			pAnimBlend->SetWeight(1.0f - m_arrLayerInfo[nLayerID].m_fFadeFactor);

			m_arrLayerInfo[nLayerID].m_pPreAnimation = pAnimBlend;
		}

		m_arrLayerInfo[nLayerID].m_pAnimation = pAnimation;

		if(m_arrLayerInfo[nLayerID].m_pAnimation)
		{
			m_arrLayerInfo[nLayerID].m_pAnimation->Play();
			m_arrLayerInfo[nLayerID].m_pAnimation->SetFrame(0);
		}


		AnimTreeNode* pAnim = m_arrLayerInfo[nLayerID].m_pAnimation.get();
		if (pAnim && pAnim->GetAnimCallBack())
		{
			pAnim->GetAnimCallBack()->OnEnter(pAnim);
		}

		if (m_arrLayerInfo[nLayerID].m_pAnimation)
		{
			m_arrLayerInfo[nLayerID].m_fCurFadeTime = m_arrLayerInfo[nLayerID].m_pAnimation->GetFadeTime();
		}
		else
		{
			m_arrLayerInfo[nLayerID].m_fCurFadeTime = 0;
		}

		AnimTreeNode* pPreAnim = m_arrLayerInfo[nLayerID].m_pPreAnimation.get();
		if (pPreAnim && pPreAnim->GetAnimCallBack())
		{
			pPreAnim->GetAnimCallBack()->OnLeave(pPreAnim);
		}
	}

	void AnimationComponent::Update()
	{
		float fTimeElepse = GetTimer()->GetFrameDeltaTime();
		for (UINT32 i = 0; i < m_arrLayerInfo.size(); ++i)
		{
			m_arrLayerInfo[i].m_fCurFadeTime -= fTimeElepse;
			if (m_arrLayerInfo[i].m_fCurFadeTime <= 0)
			{
				m_arrLayerInfo[i].m_fCurFadeTime = 0;
				m_arrLayerInfo[i].m_pPreAnimation = NULL;
			}

			if (m_arrLayerInfo[i].m_pAnimation)
			{
				m_arrLayerInfo[i].m_pAnimation->AdvanceTime(fTimeElepse);
			}
		}

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

	bool AnimationComponent::SetAnimation(AnimTreeNode* pAnim,uint32 nLayerID)
	{
		AutoLock lock(m_csParallelUpdate);

		if (m_arrLayerInfo.size() <= nLayerID)
		{
			m_arrLayerInfo.resize(nLayerID + 1);
		}

		if (m_arrLayerInfo[nLayerID].m_pAnimation == pAnim)
			return false;

		PlayAnimation(pAnim,nLayerID);

		m_bLoadOver = false;
		return IsReady();
	}


	bool AnimationComponent::SetAnimation(const char* pszName,uint32 nLayerID)
	{
		return SetAnimation( AnimTreeNode::AnimNameToID(pszName), nLayerID, pszName );
	}

	bool AnimationComponent::SetAnimation(uint32 nAnimID,uint32 nLayerID,const char* pszName)
	{
		AutoLock lock(m_csParallelUpdate);

		ASSERT(nAnimID != -1);
		if (nAnimID == -1)
			return false;

		if (m_arrLayerInfo.size() <= nLayerID)
		{
			m_arrLayerInfo.resize(nLayerID + 1);
		}

		m_arrLayerInfo[nLayerID].m_nAnimID = nAnimID;
		m_arrLayerInfo[nLayerID].m_strAnimName = pszName ? pszName : "";

		m_bLoadOver = false;
		return IsReady();
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
		evalContext.m_pNodePos = m_pose.get();
		evalContext.m_refNodePos = pRefPose;

		for (UINT32 i = 0; i < m_arrLayerInfo.size(); ++i)
		{
			if(m_arrLayerInfo[i].m_pAnimation == NULL)
				continue;

			float fFadeTime = m_arrLayerInfo[i].m_pAnimation->GetFadeTime(); 
			ASSERT(m_arrLayerInfo[i].m_fCurFadeTime <= fFadeTime);
			float fFadeFactor = fFadeTime <= 0 ? 0 : m_arrLayerInfo[i].m_fCurFadeTime / fFadeTime;
			if (m_arrLayerInfo[i].m_pPreAnimation == NULL)
				fFadeFactor = 0;

			m_arrLayerInfo[i].m_fFadeFactor = fFadeFactor;

			if (m_arrLayerInfo[i].m_pPreAnimation && fFadeFactor > 0)
			{
				m_arrLayerInfo[i].m_pPreAnimation->EvaluateAnimation(&evalContext,fFadeFactor,m_arrLayerInfo[i].m_pBoneSet.get());

				m_arrLayerInfo[i].m_pAnimation->EvaluateAnimation(&evalContext,1.0f - fFadeFactor,m_arrLayerInfo[i].m_pBoneSet.get());
			}
			else
			{
				m_arrLayerInfo[i].m_pAnimation->EvaluateAnimation(&evalContext,1.0,m_arrLayerInfo[i].m_pBoneSet.get());
			}
		}

		for (UINT32 i = 0; i < evalContext.m_arrFirst.size(); ++i)
		{
			if (evalContext.m_arrFirst[i])
			{
				evalContext.m_arrTSFPS[i] = pRefPose->GetTransformPS(i);
			}
		}

		m_pose->SetTransformPSAll(evalContext.m_arrTSFPS);

		////////////// Do IK
		for (UINT32 i = 0; i < m_arrLayerInfo.size(); ++i)
		{
			float fFadeFactor = m_arrLayerInfo[i].m_fFadeFactor;

			// Do IK
			if (m_arrLayerInfo[i].m_pPreAnimation && fFadeFactor > 0)
			{
				m_arrLayerInfo[i].m_pPreAnimation->ProcessPoseModifier(m_pose.get(),m_pSkeleton.get(),fFadeFactor);
			}

			if(m_arrLayerInfo[i].m_pAnimation)
				m_arrLayerInfo[i].m_pAnimation->ProcessPoseModifier(m_pose.get(),m_pSkeleton.get(),fFadeFactor);
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
