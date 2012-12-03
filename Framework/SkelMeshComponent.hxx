#include "Framework/SkelMeshComponent.h"

namespace ma
{

	SkelMeshComponent::SkelMeshComponent()
	{
		m_pSkelRes = NULL;
	}

	SkelMeshComponent::~SkelMeshComponent()
	{

	}

// 	void SkelMeshComponent::Render()
// 	{
// 		for (UINT i = 0; i < m_vMeshComp.size(); ++i)
// 		{
// 			if (m_vMeshComp[i] == NULL)
// 				continue;
// 			
// 			// 		IRender* pRender = ma::GetRender();
// 			//m_vMeshComp[i]->Render();
// 		}
// 	}

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

}