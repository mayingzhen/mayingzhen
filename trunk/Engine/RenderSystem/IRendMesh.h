#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

#include "Engine/RenderSystem/IRenderItem.h"

namespace ma
{
	struct MeshData;

	class ENGINE_API IRendMesh : public IRenderItem
	{
	public:
		virtual bool InitWithData(MeshData* pMeshData) = 0;
		
		virtual MeshData* GetMeshData()  = 0; 

		virtual	void GetBoundingAABB(Vector3& vMin,Vector3 &vMax) = 0;

	//protected:
		//MeshData* m_pMesData;
	};
}

#endif
