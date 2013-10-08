#include "AnimationObject.h"
#include "AnimationSet.h"
#include "Action.h"
#include "AnimationUtil.h"

namespace ma
{
	AnimationObject::AnimationObject(const char* pszSkePath,const char* pszAniSetPath)
	{
		m_pSkelAnim = NULL;
		m_pAnimSet = NULL;
		m_pSkeleton = NULL;
		m_pose = NULL;
		m_arrSkinMatrix = NULL;

		m_pAnimSet = new AnimationSet(this);

		CreateSkeleton(pszSkePath);
		CreateAniSet(pszAniSetPath);
	}

	AnimationObject::~AnimationObject()
	{
		SAFE_DELETE_ARRAY(m_arrSkinMatrix);
	}

	void AnimationObject::CreateSkeleton(const char* pSkePath)
	{
		ASSERT(pSkePath);
		if (pSkePath == NULL)
			return ;

		m_pSkeleton = LoadResourceSync<Skeleton>(pSkePath);

		const SkeletonPose* pRefPose = m_pSkeleton ? m_pSkeleton->GetResPose() : NULL;
		m_pose = pRefPose ? pRefPose->Clone() : NULL;

		int nBone = m_pSkeleton->GetBoneNumer();
		m_arrSkinMatrix = new Matrix4x4[nBone];
		for (UINT i = 0; i < nBone; ++i)
		{
			MatrixIdentity(&m_arrSkinMatrix[i]);
		}
	}

	void AnimationObject::CreateAniSet(const char* pszAniSetPath)
	{
		//ASSERT(pszAniSetPath);
		if (pszAniSetPath == NULL)
			return;

		std::string fullPath = FileSystem::getFullPath(pszAniSetPath);

		XMLInputArchive inAr;
		bool res = inAr.Open(fullPath.c_str());
		if (!res)
		{
			inAr.Close();
			return;
		}


		SAFE_DELETE(m_pAnimSet);
		m_pAnimSet = new AnimationSet(this);
		m_pAnimSet->Serialize(inAr);
		
		inAr.Close();
	}

	void AnimationObject::PlayAnimation(Action* pSkelAnim)
	{
		m_pSkelAnim = pSkelAnim;
	}

	void AnimationObject::PlayAnimation(ActionID actionID)
	{
		if (m_pAnimSet == NULL)
			return;

		if (actionID < 0 || actionID >= m_pAnimSet->GetActionNumber())
			return;

		m_pSkelAnim = (Action*)m_pAnimSet->GetActionByIndex(actionID);
	}


	void AnimationObject::PlayAnimation(const char* pszAnimName)
	{
		if (m_pAnimSet == NULL)
			return;

		m_pSkelAnim = (Action*)m_pAnimSet->GetActionByName(pszAnimName);
	}

	void AnimationObject::AdvanceTime(float fTimeElepse)
	{
		if (m_pSkelAnim)
		{
			m_pSkelAnim->AdvanceTime(fTimeElepse);
		}
	}

	void AnimationObject::EvaluateAnimation(float fWeight)
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

	void AnimationObject::SetFrame(float fFrame)
	{
		if (m_pSkelAnim)
		{
			m_pSkelAnim->SetFrame(fFrame);
		}
	}

}
