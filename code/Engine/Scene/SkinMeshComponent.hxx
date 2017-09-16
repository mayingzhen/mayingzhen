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
		
		m_arrLodRenderable.clear();
		for (UINT iLod = 0; iLod < m_vecMesData.size(); ++iLod)
		{
			MeshData* pMeshData = m_vecMesData[iLod].get();

			VEC_RENDERABLE arrRenderable;
			for (UINT iSub = 0; iSub < pMeshData->GetSubMeshNumber(); ++iSub)
			{
				SkinMeshRenderable* pRenderable = new SkinMeshRenderable();

				pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
				pRenderable->m_pVertexBuffer = pMeshData->GetVertexBuffer(); 
				pRenderable->m_pIndexBuffer = pMeshData->GetIndexBuffer();
				pRenderable->m_pSubMeshData = pMeshData->GetSubMeshByIndex(iSub);

				SubMaterial* pSubMaterial = m_pMaterial->GetLodSubByIndex(iLod, iSub);
				
				Technique* pTech = pSubMaterial->GetShadingTechnqiue();

				Vector3 pos_extent = pMeshData->GetBoundingAABB().getHalfSize();
				Vector3 pos_center = pMeshData->GetBoundingAABB().getCenter();
				Vector2 tc_extent = pMeshData->GetUVBoundingAABB().getHalfSize();
				Vector2	tc_center = pMeshData->GetUVBoundingAABB().getCenter();
				Vector4 tc_extent_center = Vector4(tc_extent.x, tc_extent.y, tc_center.x, tc_center.y);

				pTech->SetValue(pTech->GetUniform("pos_extent"), pos_extent);
				pTech->SetValue(pTech->GetUniform("pos_center"), pos_center);
				pTech->SetValue(pTech->GetUniform("tc_extent_center"), tc_extent_center);

				pRenderable->m_pSubMaterial = pSubMaterial;

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

