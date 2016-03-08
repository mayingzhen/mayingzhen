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
		ASSERT(m_pMaterial && !m_vecMesData.empty());
		if (m_pMaterial == NULL || m_vecMesData.empty())
			return;
		
		for (UINT iLod = 0; iLod < m_vecMesData.size(); ++iLod)
		{
			MeshData* pMeshData = m_vecMesData[iLod].get();

			VEC_RENDERABLE arrRenderable;
			for (UINT iSub = 0; iSub < pMeshData->GetSubMeshNumber(); ++iSub)
			{
				SkinMeshRenderable* pRenderable = new SkinMeshRenderable();

				pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
				pRenderable->m_pDeclaration = pMeshData->GetVertexDeclar(); 
				pRenderable->m_pVertexBuffer = pMeshData->GetVertexBuffer(); 
				pRenderable->m_pIndexBuffer = pMeshData->GetIndexBuffer();
				pRenderable->m_pSubMeshData = pMeshData->GetSubMeshByIndex(iSub);
				pRenderable->m_posAABB = pMeshData->GetBoundingAABB();
				pRenderable->m_tcAABB = pMeshData->GetUVBoundingAABB();

				pRenderable->m_pSubMaterial = m_pMaterial->GetSubMaterialByIndex(iLod,iSub);

				arrRenderable.push_back(pRenderable);
			}
			m_arrLodRenderable.push_back(arrRenderable);
		}

		SetAABB(m_vecMesData[0]->GetBoundingAABB());
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

