#include "D3D9RendMesh.h"
#include "DxMeshHelp.h"

namespace ma
{
	D3D9RendMesh::D3D9RendMesh()
	{
		m_pD3DMesh = NULL;
		m_pMeshData = NULL;
	}

	D3D9RendMesh::~D3D9RendMesh()
	{
	}

	bool D3D9RendMesh::InitWithData(MeshData* pMeshData)
	{
		D3D9Render* pDxRender = (D3D9Render*)GetRender();
		D3D9RenderDevice* pDxRenderDevice = (D3D9RenderDevice*)GetRenderDevice();

		m_pD3DMesh = CreateD3DMesh( pDxRenderDevice->GetDXDevive(), pMeshData);

		m_pMeshData = pMeshData;

		return true;
	}

	void D3D9RendMesh::GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3 &vMax)
	{
		if (m_pMeshData)
		{
			m_pMeshData->GetBoundingAABB(&vMin,&vMax);
		}
	}
}
