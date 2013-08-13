#include "AnimComponent.h"

namespace ma
{
	IMPL_OBJECT(AnimComponent,Component)

	AnimComponent::AnimComponent(GameObject* pGameObj)
	:Component(pGameObj)
	{
	}

	AnimComponent::~AnimComponent()
	{
	}

	void AnimComponent::Start()
	{
		UINT nMeshComp = m_pGameObject->GetTypeComponentNumber<MeshComponent>();
		for (UINT i = 0; i < nMeshComp; ++i)
		{
			MeshComponent* pMeshComp = m_pGameObject->GetTypeComponentByIndex<MeshComponent>(i);
			ASSERT(pMeshComp->GetRendMesh());
			if (pMeshComp->GetRendMesh() == NULL)
				continue;

			m_arrRenderMesh.push_back(pMeshComp->GetRendMesh())	;
		}

	}

	void AnimComponent::Stop()
	{
		m_arrRenderMesh.clear();
	}

	void AnimComponent::Update()
	{
		Matrix4x4* skinMatrix = m_pAnimtionObject->GetSkinMatrixArray();
 		UINT nNumber = m_pAnimtionObject->GetSkinMatrixNumber();
		for (UINT i = 0; i < m_arrRenderMesh.size(); ++i)
		{
			if (m_arrRenderMesh[i] == NULL)
				continue;

			m_arrRenderMesh[i]->SetSkinMatrix(skinMatrix,nNumber);
		}
	}


	void AnimComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		if (GetAnimationSystem() == NULL)
			return;
		
		if (pszSkeletonPath)
		{
 			m_pAnimtionObject = GetAnimationSystem()->CreateAnimationObject(pszSkeletonPath);
		}
	}

	void AnimComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

 		std::string sAniSetPath,sSkeletonPath;
 	
 		IAnimationSet* pAnimationSet = m_pAnimtionObject->GetAnimationSet();
// 		if (pAnimationSet)
 //		{
// 			sAniSetPath = pAnimationSet->GetResPath();
// 		}
// 
// 		ISkeleton* pSkeleton = m_pAnimtionObject->GetSkeleton();
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