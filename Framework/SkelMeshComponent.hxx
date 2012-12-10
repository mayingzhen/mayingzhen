#include "Framework/SkelMeshComponent.h"

namespace ma
{

	SkelMeshComponent::SkelMeshComponent()
	{
		m_pSkelRes = NULL;
		m_pose = new NodePose;
		m_pAnimationInst = NULL;

		//m_pSkeletonAnimation = new SkeletonAnimation();
	}

	SkelMeshComponent::~SkelMeshComponent()
	{
		SAFE_DELETE(m_pose);
		//SAFE_DELETE(m_pSkeletonAnimation);
	}

	void SkelMeshComponent::Update()
	{
		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		Skeleton* pSkeleton = m_pSkelRes->GetSkeleton();
		const NodePose* refPose = pSkeleton ? pSkeleton->GetResPose() : NULL;
		if (!pSkeleton || !refPose)
			return;

		//m_pAnimationInst->AdvanceTime(fTimeElapsed);
		m_pAnimNode->AdvanceTime(fTimeElapsed);
		
		AnimEvalContext evalConst;
		
		maNodeTransform tsfIdent;
		maTransformSetIdentity(&tsfIdent);
		evalConst.m_arrTSFLS.resize(refPose->GetNodeNumber(),tsfIdent);
		//memset(&arrLSTSF[0],0,sizeof(maNodeTransform)*arrLSTSF.size());
		//m_pAnimationInst->EvaluateAnimation(tsfIdent);
		m_pAnimNode->EvaluateAnimation(&evalConst,1.0f);

		for (UINT i = 0; i < m_pose->GetNodeNumber(); ++i)
		{
			maNodeTransform tsfPS;
			maTransfromMul(&tsfPS,&evalConst.m_arrTSFLS[i],&refPose->GetTransformPS(i));
			m_pose->SetTransformPS(&tsfPS,i);
			//m_pose->SetTransformPS(&arrLSTSF[i],i);
		}

		m_pose->SyncObjectSpace();
	}

	void SkelMeshComponent::Render()
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		if (m_pGameObject == NULL)
			return;

		D3DXMATRIX worldmat = m_pGameObject->GetWorldMatrix();

		xmMatrix4x4 arrSkinMatrix[256];
		Skeleton* pSkeleton = m_pSkelRes->GetSkeleton();
		UINT nBoneNum = pSkeleton->GetBoneNumer();
		for (UINT i = 0; i < nBoneNum; ++i)
		{
			//D3DXMatrixIdentity(&arrSkinMatrix[i]);
			maMatrixFromTransform(&arrSkinMatrix[i],& m_pose->GetTransformOS(i));
			D3DXMatrixMultiply(&arrSkinMatrix[i],& pSkeleton->GetBoneMatrixOSInv(i),&arrSkinMatrix[i]);
		}

		for (UINT i = 0; i < m_vMeshComp.size(); ++i)
		{
			if (m_vMeshComp[i] == NULL)
				continue;
			
			MeshRes* pMeshRes = m_vMeshComp[i]->GetMeshRes();
			Texture* pTexture = m_vMeshComp[i]->GetTexture();
			if (pMeshRes == NULL)
				continue;
			
			pRender->RenderSkelMesh(arrSkinMatrix,nBoneNum,&worldmat,pMeshRes->GetRendMesh(),pTexture->GetRendTexture());
		}
	}

	void SkelMeshComponent::AddMeshComp(MeshComponent* pMeshComp)
	{
		if (pMeshComp == NULL)
			return;

		std::vector<MeshComponent*>::iterator it = std::find(m_vMeshComp.begin(),m_vMeshComp.end(),pMeshComp);
		if (it != m_vMeshComp.end())
			return;

		m_vMeshComp.push_back(pMeshComp);
	}

	Skeleton* SkelMeshComponent::LoadSkeleton(const char* pSkelPath)
	{
		m_pSkelRes = new SkeletonRes(pSkelPath);
		m_pSkelRes->Load();

		Skeleton* pSkeleton = m_pSkelRes->GetSkeleton();
		const NodePose* pRefPose = pSkeleton ? pSkeleton->GetResPose() : NULL;
		m_pose = pRefPose ? pRefPose->Clone() : NULL;
		return m_pSkelRes->GetSkeleton();
	}

	void SkelMeshComponent::PlayAnimation(const char* pszAniName)
	{
		//m_pAnimationSet->
	}

	void SkelMeshComponent::PlayAnimation(Animation* pAnimation)
	{
		if (pAnimation == NULL)
			return;
		
		//m_pSkeletonAnimation->Instantiate(pAnimation);
		
	}

	AnimationInst* SkelMeshComponent::LoadAnimation(const char* pAniPath,Skeleton* pSkeleton)
	{
		m_pAniRes = new AnimationRes(pAniPath);
		m_pAniRes->Load();

		m_pAnimationInst = new AnimationInst(m_pAniRes->GetAimation(),pSkeleton);
		return m_pAnimationInst;
	}

}