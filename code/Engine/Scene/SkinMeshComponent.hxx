#include "SkinMeshComponent.h"
#include "../RenderSystem/RenderQueue.h"
#include "../RenderSystem/RenderThread.h"

namespace ma
{
	void SkinMeshComponent::RegisterAttribute()
	{
		REGISTER_OBJECT(SkinMeshComponent,CreateSkinMeshComponent); 

		COPY_BASE_ATTRIBUTES(SkinMeshComponent,MeshComponent);
	}


	void SkinMeshComponent::CreateRenderable()
	{
		ASSERT(m_pMaterial && m_pMesData);
		if (m_pMaterial == NULL || m_pMesData == NULL)
			return;
		
		for (UINT iLod = 0; iLod < m_pMesData->GetLodNumerber(); ++iLod)
		{
			VEC_RENDERABLE arrRenderable;
			for (UINT iSub = 0; iSub < m_pMesData->GetSubMeshNumber(iLod); ++iSub)
			{
				SkinMeshRenderable* pRenderable = new SkinMeshRenderable();

				pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
				pRenderable->m_pDeclaration = m_pMesData->GetVertexDeclar(); 
				pRenderable->m_pVertexBuffer = m_pMesData->GetVertexBuffer(); 
				pRenderable->m_pIndexBuffer = m_pMesData->GetIndexBuffer();
				pRenderable->m_pSubMeshData = m_pMesData->GetSubMeshByIndex(iLod,iSub);
				pRenderable->m_pSubMaterial = m_pMaterial->GetSubMaterialByIndex(iLod,iSub);
				pRenderable->m_posAABB = m_pMesData->GetBoundingAABB();
				pRenderable->m_tcAABB = m_pMesData->GetUVBoundingAABB();

				arrRenderable.push_back(pRenderable);
			}
			m_arrLodRenderable.push_back(arrRenderable);
		}

		SetAABB(m_pMesData->GetBoundingAABB());
	}

	void SkinMeshComponent::SetSkinMatrix(const Matrix3x4* arrMatrixs,uint32 nCount)
	{
		profile_code();
		
		uint32 nLod = 0;
		for (UINT i = 0; i < m_arrLodRenderable[nLod].size(); ++i)
		{
			SkinMeshRenderable* pSkinRenderable = (SkinMeshRenderable*)m_arrLodRenderable[nLod][i].get();
			pSkinRenderable->SetSkinMatrix(arrMatrixs,nCount);
		}
	}

	RefPtr<SkinMeshComponent> CreateSkinMeshComponent()
	{
		return new SkinMeshComponent();
	}

}

