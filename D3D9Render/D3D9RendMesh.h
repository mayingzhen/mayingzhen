#ifndef  _DXRENDMESH__H__
#define  _DXRENDMESH__H__

#include "Engine/RenderSystem/IRendMesh.h"

namespace ma
{
	class MeshData;

	class D3D9RENDER_API D3D9RendMesh : public IRendMesh
	{
	public:
		D3D9RendMesh();

		~D3D9RendMesh();

		ID3DXMesh* GetD3DXMesh() {return m_pD3DMesh;}
		
		MeshData* GetMeshData() {return m_pMeshData;}

		virtual	bool InitWithData(MeshData* pMeshData);

		virtual	void GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3 &vMax);

	private:
		ID3DXMesh*	m_pD3DMesh;
		MeshData* m_pMeshData;
	};
}


#endif
