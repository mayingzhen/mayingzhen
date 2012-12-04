#include "Framework/SkelMeshComponent.h"

namespace ma
{

	SkelMeshComponent::SkelMeshComponent()
	{
		m_pSkelRes = NULL;
		m_pose = new NodePose;
	}

	SkelMeshComponent::~SkelMeshComponent()
	{
		SAFE_DELETE(m_pose);
	}

	void SkelMeshComponent::Update()
	{
		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		Animation* pAnimtion = m_pAniRes->GetAimation();
		Skeleton* pSkeleton = m_pSkelRes->GetSkeleton();
		if (pAnimtion && pSkeleton)
		{
			pAnimtion->AdvanceTime(fTimeElapsed);
		
			std::vector<maNodeTransform> arrLSTSF;
			pAnimtion->EvaluateAnimation(arrLSTSF);

			//------------------------------------------------------------------------------
			//Local space animation to parent space
			//------------------------------------------------------------------------------
			m_pose->InitLocalSpace(arrLSTSF,pSkeleton->GetResPose());
		}
		
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

	void SkelMeshComponent::LoadSkeleton(const char* pSkelPath)
	{
		m_pSkelRes = new SkeletonRes(pSkelPath);
		m_pSkelRes->Load();
	}

	void SkelMeshComponent::LoadAnimation(const char* pAniPath)
	{
		m_pAniRes = new AnimationRes(pAniPath);
		m_pAniRes->Load();
	}

}