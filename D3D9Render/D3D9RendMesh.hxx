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
		if (pMeshData == NULL)
			return false;

		D3D9RenderDevice* pDxRenderDevice = (D3D9RenderDevice*)GetRenderDevice();

		m_pD3DMesh = CreateD3DMesh( pDxRenderDevice->GetDXDevive(), pMeshData);

		m_pMeshData = pMeshData;

		return true;
	}

	void D3D9RendMesh::GetBoundingAABB(Vector3& vMin,Vector3 &vMax)
	{
		if (m_pMeshData)
		{
			m_pMeshData->GetBoundingAABB(&vMin,&vMax);
		}
	}
}
