#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

#include "Engine/RenderSystem/IRendITem.h"

namespace ma
{
	struct MeshData;

	class ENGINE_API IRendMesh : IRendITem
	{
	public:
		virtual bool InitWithData(MeshData* pMeshData) = 0;
		
		virtual MeshData* GetMeshData()  = 0; 

		virtual	void GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3 &vMax) = 0;

	//protected:
		//MeshData* m_pMesData;
	};
}

#endif
