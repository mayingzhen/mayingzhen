#include "DXRender/DxRendMesh.h"

namespace ma
{

	DxRendMesh::DxRendMesh()
	{
		m_pD3DMesh = NULL;
	}

	DxRendMesh::~DxRendMesh()
	{

	}

	bool DxRendMesh::Load(const char* pszPath)
	{
		DxRender* pDxRender = (DxRender*)GetRender();

		HRESULT hr = D3DXLoadMeshFromX(pszPath,D3DXMESH_MANAGED,pDxRender->GetDXDevive(),
			NULL,NULL,NULL,NULL,&m_pD3DMesh);

		return hr == D3D_OK;
	}
}
