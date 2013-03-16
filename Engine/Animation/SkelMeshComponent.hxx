#include "SkelMeshComponent.h"

namespace ma
{
	IMPL_OBJECT(SkelMeshComponent,Component)

	SkelMeshComponent::SkelMeshComponent()
	{
		m_pAnimtionPlay= NULL;
		if (GetAnimationDevice() == NULL)
		{	
			m_pAnimtionPlay = GetAnimationDevice()->CreateAnimationPlay();
		}
	}

	SkelMeshComponent::~SkelMeshComponent()
	{
		//SAFE_DELETE(m_pAnimtionPlay);
	}

	void SkelMeshComponent::Update()
	{
		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		if (m_pAnimtionPlay == NULL)
			return;

		m_pAnimtionPlay->AdvanceTime(fTimeElapsed);

		/// Parallel Update
 		m_pAnimtionPlay->EvaluateAnimation(1.0f);
	}

	void SkelMeshComponent::Render()
	{
		if (GetRenderDevice() == NULL)
			return;

		if (m_pGameObject == NULL || m_pAnimtionPlay == NULL)
			return;

		const D3DXMATRIX& worldmat = m_pGameObject->GetWorldMatrix();
		const D3DXMATRIX* arrSkinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
		UINT nSkinMaxtrixNum = m_pAnimtionPlay->GetSkinMatrixNumber();

		for (UINT i = 0; i < m_vMeshComp.size(); ++i)
		{
			if (m_vMeshComp[i] == NULL)
				continue;
			
			IRendMesh* pMeshRes = m_vMeshComp[i]->GetRendMesh();
			IRendTexture* pTexture = m_vMeshComp[i]->GetTexture();
			if (pMeshRes == NULL)
				continue;
			
			//GetRenderDevice()->RenderSkelMesh(arrSkinMatrix,nSkinMaxtrixNum,&worldmat,pMeshRes,pTexture);
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

// 	void SkelMeshComponent::SetSkeleton(Skeleton* pSkeleton)
// 	{
// 		m_pSkeleton = pSkeleton;
// 		m_pAnimtionPlay->SetSkeleton(m_pSkeleton);
// 	}
// 
// 	void SkelMeshComponent::SetAnimationSet(AnimationSet* pAnimSet)
// 	{
// 		m_pAnimSet = pAnimSet;
// 	}

	void SkelMeshComponent::PlayAnimation(const char* pszAniName)
	{
		if (pszAniName == NULL)
			return;

		if (m_pAnimtionPlay == NULL)
			return;

		m_pAnimtionPlay->PlayAnimation(pszAniName);
	}

	void SkelMeshComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		if (GetAnimationDevice() == NULL)
			return;

		IAnimationSet* pAnimationSet = GetAnimationDevice()->CreateAnimationSet();
		pAnimationSet->Load(pszSkeletonPath);

		SkeletonData* pSkeletonData = LoadSkeletonFromBinaryFile(pszSkeletonPath);
		ISkeleton* pSkeleton = GetAnimationDevice()->CreateSkeleton();
		pSkeleton->InitWithData(pSkeletonData);
	}

	void SkelMeshComponent::Serialize(SerializeListener& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.SerializeObjectArray(m_vMeshComp);

		std::string sAniSetPath,sSkeletonPath;
	
		IAnimationSet* pAnimationSet = m_pAnimtionPlay->GetAnimationSet();
		if (pAnimationSet)
		{
			sAniSetPath = pAnimationSet->GetResPath();
		}

		ISkeleton* pSkeleton = m_pAnimtionPlay->GetSkeleton();
		if (pSkeleton && pSkeleton->GetSkeletonData())
		{
			sSkeletonPath = pSkeleton->GetSkeletonData()->GetResPath();
		}
				
		sl.Serialize(sAniSetPath);
		sl.Serialize(sSkeletonPath);

		if ( sl.IsReading() )
		{
			Load(sAniSetPath.c_str(),sSkeletonPath.c_str());
		}

		sl.EndSection();
	}
}