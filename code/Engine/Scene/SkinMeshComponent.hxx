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

	RefPtr<MeshRenderable> SkinMeshComponent::CreateMeshRenderable()
	{
		return new SkinMeshRenderable();
	}

	void SkinMeshComponent::SetSkinMatrix(const Matrix3x4* arrMatrixs,uint32_t nCount)
	{
		profile_code();
		
		uint32_t nLod = 0;
		for (uint32_t i = 0; i < m_arrLodRenderable[nLod].size(); ++i)
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

