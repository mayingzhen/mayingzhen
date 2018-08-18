#pragma once

#include "Engine/Scene/MeshComponent.h"

namespace ma
{
	class SkinMeshComponent : public MeshComponent
	{
		
	public:

		DECL_OBJECT(SkinMeshComponent)	

		static void					RegisterAttribute();

		void						SetSkinMatrix(const Matrix3x4* arrMatrixs,uint32_t nCount);
	
	private:
		virtual RefPtr<MeshRenderable>	CreateMeshRenderable();
	};

	RefPtr<SkinMeshComponent> CreateSkinMeshComponent();
}

