#include "AnimPlayComponent.h"

namespace ma
{
	IMPL_OBJECT(AnimPlayComponent,Component)

	AnimPlayComponent::AnimPlayComponent(GameObject* pGameObj)
	:Component(pGameObj)
	{
		m_pAnimtionPlay= NULL;
		if (GetAnimationDevice())
		{	
			m_pAnimtionPlay = GetAnimationDevice()->CreateAnimationPlay();
		}
	}

	AnimPlayComponent::~AnimPlayComponent()
	{
		//SAFE_DELETE(m_pAnimtionPlay);
		if (GetAnimationDevice())
		{
			//GetAnimationDevice()->
		}
	}

	void AnimPlayComponent::Start()
	{
		UINT nMeshComp = m_pGameObject->GetTypeComponentNumber<MeshComponent>();
		for (UINT i = 0; i < nMeshComp; ++i)
		{
			MeshComponent* pMeshComp = m_pGameObject->GetTypeComponentByIndex<MeshComponent>(i);
			m_arrRenderMesh.push_back(pMeshComp->GetRendMesh())	;
		}

	}

	void AnimPlayComponent::Stop()
	{
		m_arrRenderMesh.clear();
	}

	void AnimPlayComponent::Update()
	{
		if (ma::GetTimer() == NULL)
			return;

		float fTimeElapsed = ma::GetTimer()->GetFrameDeltaTime();

		if (m_pAnimtionPlay == NULL)
			return;

		m_pAnimtionPlay->AdvanceTime(fTimeElapsed);

		/// Parallel Update
 		m_pAnimtionPlay->EvaluateAnimation(1.0f);

		Matrix4x4* skinMatrix = m_pAnimtionPlay->GetSkinMatrixArray();
 		UINT nNumber = m_pAnimtionPlay->GetSkinMatrixNumber();
		for (UINT i = 0; i < m_arrRenderMesh.size(); ++i)
		{
			m_arrRenderMesh[i]->SetSkinMatrix(skinMatrix,nNumber);
		}
	}


	void AnimPlayComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		if (GetAnimationDevice() == NULL)
			return;

		m_pAnimtionPlay->CreateSkeleton(pszSkeletonPath);
		m_pAnimtionPlay->CreateAnimSet(pszAniSetPath);

	}

	void AnimPlayComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

// 		sl.SerializeObjectArray(m_vMeshComp);
// 
// 		std::string sAniSetPath,sSkeletonPath;
// 	
// 		IAnimationSet* pAnimationSet = m_pAnimtionPlay->GetAnimationSet();
// 		if (pAnimationSet)
// 		{
// 			sAniSetPath = pAnimationSet->GetResPath();
// 		}
// 
// 		ISkeleton* pSkeleton = m_pAnimtionPlay->GetSkeleton();
// 		if (pSkeleton && pSkeleton->GetSkeletonData())
// 		{
// 			sSkeletonPath = pSkeleton->GetSkeletonData()->GetResPath();
// 		}
// 				
// 		sl.Serialize(sAniSetPath);
// 		sl.Serialize(sSkeletonPath);
// 
// 		if ( sl.IsReading() )
// 		{
// 			Load(sAniSetPath.c_str(),sSkeletonPath.c_str());
// 		}

		sl.EndSection();
	}
}