#include "Framework/SkelMeshComponent.h"

namespace ma
{
	IMPL_OBJECT(SkelMeshComponent,Component)

	SkelMeshComponent::SkelMeshComponent()
	{
		m_pAnimSet = NULL;
		m_pAnimtionPlay = new AnimationPlay();
	}

	SkelMeshComponent::~SkelMeshComponent()
	{
		SAFE_DELETE(m_pAnimtionPlay);
	}

	void SkelMeshComponent::Update()
	{
		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		m_pAnimtionPlay->AdvanceTime(fTimeElapsed);

		/// Parallel Update
 		m_pAnimtionPlay->EvaluateAnimation(1.0f);
	}

	void SkelMeshComponent::Render()
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		if (m_pGameObject == NULL)
			return;

		D3DXMATRIX worldmat = m_pGameObject->GetWorldMatrix();

		D3DXMATRIX arrSkinMatrix[256];
		UINT nBoneNum = m_pSkeleton->GetBoneNumer();
		NodePose* pAnimPose = m_pAnimtionPlay->GetAnimationPose();
		for (UINT i = 0; i < nBoneNum; ++i)
		{
			if (pAnimPose)
			{
				maMatrixFromTransform(&arrSkinMatrix[i],& pAnimPose->GetTransformOS(i));
				D3DXMatrixMultiply(&arrSkinMatrix[i],& m_pSkeleton->GetBoneMatrixOSInv(i),&arrSkinMatrix[i]);
			}
			else
			{
				D3DXMatrixIdentity(&arrSkinMatrix[i]);
			}
		}

		for (UINT i = 0; i < m_vMeshComp.size(); ++i)
		{
			if (m_vMeshComp[i] == NULL)
				continue;
			
			IRendMesh* pMeshRes = m_vMeshComp[i]->GetMeshRes();
			IRendTexture* pTexture = m_vMeshComp[i]->GetTexture();
			if (pMeshRes == NULL)
				continue;
			
			pRender->RenderSkelMesh(arrSkinMatrix,nBoneNum,&worldmat,pMeshRes,pTexture);
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

	void SkelMeshComponent::SetSkeleton(Skeleton* pSkeleton)
	{
		m_pSkeleton = pSkeleton;
		m_pAnimtionPlay->SetSkeleton(m_pSkeleton);
	}

	void SkelMeshComponent::SetAnimationSet(AnimationSet* pAnimSet)
	{
		m_pAnimSet = pAnimSet;
	}

	void SkelMeshComponent::PlayAnimation(const char* pszAniName)
	{
		if (pszAniName == NULL)
			return;

		if (m_pAnimSet == NULL)
			return;

		AnimationAction* pSkelAnim = m_pAnimSet->GetAnimationActionByName(pszAniName);

		m_pAnimtionPlay->PlayAnimation(pSkelAnim);
	}
}