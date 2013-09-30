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


	void AnimComponent::BuildRenderItem()
	{
		UINT nMeshComp = m_pGameObject->GetTypeComponentNumber<MeshComponent>();
		for (UINT i = 0; i < nMeshComp; ++i)
		{
			MeshComponent* pMeshComp = m_pGameObject->GetTypeComponentByIndex<MeshComponent>(i);
			RenderMesh* pRenderMesh = pMeshComp ? pMeshComp->GetRendMesh() : NULL;
			ASSERT(pRenderMesh);
			if (pRenderMesh == NULL)
				continue;

			Matrix4x4* skinMatrix = m_pAnimtionObject->GetSkinMatrixArray();
			UINT nNumber = m_pAnimtionObject->GetSkinMatrixNumber();
			pRenderMesh->SetSkinMatrix(skinMatrix,nNumber);
		}
	}


	void AnimComponent::Load(const char* pszAniSetPath, const char* pszSkeletonPath)
	{
		if (GetAnimationSystem() == NULL)
			return;

		m_strAnimaSetPath = pszAniSetPath ? pszAniSetPath : "";
		m_strSkeletonPath = pszSkeletonPath ? pszSkeletonPath : "";
		
		if (pszSkeletonPath)
		{
 			m_pAnimtionObject = GetAnimationSystem()->CreateAnimationObject(pszSkeletonPath);
		}

		if (pszAniSetPath)
		{
			//m_strAnimaSetPath = GetAnimationSystem()->C
		}
	}

	void AnimComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);
				
		sl.Serialize(m_strAnimaSetPath);
		sl.Serialize(m_strSkeletonPath);

		if ( sl.IsReading() )
		{
			Load(m_strAnimaSetPath.c_str(),m_strSkeletonPath.c_str());
		}

		sl.EndSection();
	}
}