#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

#include "Common/IRender/IRendITem.h"

namespace ma
{
	struct MeshData;

	class COMMON_API IRendMesh : IRendITem
	{
	public:
		virtual bool InitWithData(MeshData* pMeshData) = 0;
		
		virtual	void GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3 &vMax) = 0;
	
	//protected:
		//MeshData* m_pMesData;
	};
}

#endif
