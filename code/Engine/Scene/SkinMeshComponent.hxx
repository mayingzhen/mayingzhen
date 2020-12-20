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
		uint32_t nLod = 0;
		
		uint32_t nRenderableCount = GetMeshRenderProxy()->GetRenderableCount();
		for (uint32_t i = 0; i < nRenderableCount; ++i)
		{
			Renderable* pRenderable = GetMeshRenderProxy()->GetRenderableByIndex(i);
			SkinMeshRenderable* pSkinRenderable = (SkinMeshRenderable*)pRenderable;
			pSkinRenderable->SetSkinMatrix(arrMatrixs,nCount);
		}
	}

	RefPtr<SkinMeshComponent> CreateSkinMeshComponent()
	{
		return new SkinMeshComponent();
	}

}

