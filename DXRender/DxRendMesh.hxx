#include "DXRender/DxRendMesh.h"

namespace ma
{

	template<class IndexType, class MeshDataType>
	void CopyIndexBuffer(void* destBuf, MeshDataType* pMeshData, const xmUint nSubMeshNum, xmUint numIndex ) 
	{
		IndexType* destIndexData = reinterpret_cast<IndexType*>(destBuf);
		const IndexType* arrInd = reinterpret_cast<const IndexType*>(pMeshData->GetIndexBuffer());
		xmUint nIndexFilled = 0;
		for (xmUint nSubMeshCnt = 0; nSubMeshCnt < nSubMeshNum; ++nSubMeshCnt)
		{
			SubMeshData* pSubMeshData = pMeshData->GetSubMesh(nSubMeshCnt);
			const xmUint nVertexStart = pSubMeshData->GetVertexStart();
			const xmUint nIndexCount =  pSubMeshData->GetIndexCount();
			for (xmUint nIndCnt = 0; nIndCnt < nIndexCount; ++nIndCnt)
			{
				destIndexData[nIndexFilled] = nVertexStart + arrInd[nIndexFilled];
				++nIndexFilled;
			}
		}
		assert(nIndexFilled == numIndex); 
	}

	template<class MeshDataType>
	ID3DXMesh* CreateD3DXMeshFromData(IDirect3DDevice9* pd3dDevice, MeshDataType* pMeshData)
	{
		static D3DVERTEXELEMENT9 velem[] = 
		{
			{ 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
			{ 0, 3 * 4, D3DDECLTYPE_UBYTE4,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDINDICES,  0},
			{ 0, 4 * 4, D3DDECLTYPE_UBYTE4N,    D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDWEIGHT,   0},
			{ 0, 5 * 4, D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,    0 },
			{ 0, 8 * 4, D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
			{ 0,10 * 4, D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TANGENT,	0 },
			{ 0,13 * 4, D3DDECLTYPE_D3DCOLOR,   D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,	0 },
			{ 0,14 * 4, D3DDECLTYPE_D3DCOLOR,   D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,	1 },
			D3DDECL_END()
		};
		ID3DXMesh *pD3DMesh = NULL;
		//import mesh
		//------------------------------------------------------------------------------
		xmUint numIndex = 0;
		xmUint nIndexSize = 0;
		xmUint numVertex = 0;
		bool bIsIndex32 = false;

		bIsIndex32 = pMeshData->GetIsIndex32();
		nIndexSize = bIsIndex32 ? sizeof(xmUint32) : sizeof(xmUint16);
		numIndex = pMeshData->GetIndexNumber();
		numVertex = pMeshData->GetVertexNumber();

		//Create D3DMesh
		{
			const xmUint32 numFace = numIndex / 3;
			const xmUint nSubMeshNum = pMeshData->GetSubMeshNumber();

			xmUint32 dwUse32Ind = bIsIndex32 ? D3DXMESH_32BIT : 0;
			_D3D_V(D3DXCreateMesh(numFace,numVertex,dwUse32Ind|D3DXMESH_MANAGED,velem ,pd3dDevice,&pD3DMesh));

			void* d3dbuf;
			_D3D_V(pD3DMesh->LockIndexBuffer(0,&d3dbuf));

			if (nIndexSize == sizeof(xmUint32))
			{
				CopyIndexBuffer<xmUint32>(d3dbuf,pMeshData,nSubMeshNum, numIndex);
			}
			else
			{
				CopyIndexBuffer<xmUint16>(d3dbuf,pMeshData,nSubMeshNum, numIndex);
			}				

			_D3D_V(pD3DMesh->UnlockIndexBuffer());

			VertexType0* d3dVertexData;
			_D3D_V(pD3DMesh->LockVertexBuffer(0,(LPVOID*)&d3dVertexData));
			{
				const VertexType0* arrVertex = pMeshData->GetVertexBufferVertexType0();
				for (xmUint nVCnt = 0; nVCnt < numVertex; ++nVCnt)
				{
					d3dVertexData[nVCnt] = arrVertex[nVCnt];
				}
			}
			//memcpy(d3dVertexData,(const void*)&expMeshData.m_arrVertexBuffer[0],expMeshData.m_arrVertexBuffer.size());
			_D3D_V(pD3DMesh->UnlockVertexBuffer());

			//fill sub mesh
			{
				DWORD* adata;
				_D3D_V(pD3DMesh->LockAttributeBuffer(0,&adata));
				DWORD faceFilled = 0;


				for(xmUint nSubMeshCnt = 0; nSubMeshCnt < nSubMeshNum; ++nSubMeshCnt)
				{
					SubMeshData* pSubMeshData = pMeshData->GetSubMesh(nSubMeshCnt);
					const xmUint nTriNum = pSubMeshData->GetIndexCount() / 3;
					for (xmUint nTriCnt = 0; nTriCnt < nTriNum; ++nTriCnt)
					{
						adata[faceFilled] = nSubMeshCnt;
						++faceFilled;
					}
				}

				_D3D_V(pD3DMesh->UnlockAttributeBuffer());
			}

			{
				DWORD* aAdjacency = new DWORD[pD3DMesh->GetNumFaces() * 3];
				assert(NULL != aAdjacency);

				_D3D_V( pD3DMesh->ConvertPointRepsToAdjacency(NULL, aAdjacency) );

				_D3D_V( pD3DMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL) );
				SAFE_DELETE_ARRAY(aAdjacency);
			}

		}//Create D3DMesh

		return pD3DMesh;
	}

	ID3DXMesh** CreateD3DMesh(IDirect3DDevice9* pd3dDevice,MeshData* pMeshData, int *pTotalLod)
	{
		int nLodNumber = pMeshData->GetLODMeshNumber();
		if (nLodNumber == 0)
			return NULL;

		if (pTotalLod)
			*pTotalLod = nLodNumber;

		LPD3DXMESH*			ppD3DMesh = new LPD3DXMESH[nLodNumber];

		if (pd3dDevice)
		{
			if (nLodNumber > 1)
			{
				// Create LOD Mesh

				for (int i = 0; i < nLodNumber; ++i)
				{
					ppD3DMesh[i] = CreateD3DXMeshFromData(pd3dDevice, pMeshData->GetLODMesh(i));
				}
			}
			else
			{
				ppD3DMesh[0] = CreateD3DXMeshFromData(pd3dDevice, pMeshData);
			}
		}//if (d3dDevice)

		return ppD3DMesh;	
	}


	DxRendMesh::DxRendMesh()
	{
		m_ppD3DMesh = NULL;
		m_pMeshData = NULL;
	}

	DxRendMesh::~DxRendMesh()
	{

	}

// 	bool DxRendMesh::Load(const char* pszPath)
// 	{
// 		DxRender* pDxRender = (DxRender*)GetRender();
// 		DxRenderDevice* pDxRenderDevice = (DxRenderDevice*)GetRenderDevice();
// 
// 		//HRESULT hr = D3DXLoadMeshFromX(pszPath,D3DXMESH_MANAGED,pDxRender->GetDXDevive(),
// 		//	NULL,NULL,NULL,NULL,&m_pD3DMesh);
// 
// 		//return hr == D3D_OK;
// 
// 		m_pMeshData = ResourceBuilder::LoadMeshFromBinaryFile(pszPath);
// 
// 		Init(m_pMeshData);
// 		//int nTotalLod;
// 		//m_ppD3DMesh = CreateD3DMesh( pDxRenderDevice->GetDXDevive(), m_pMeshData, &nTotalLod );
// 
// 		return true;
// 	}

	bool DxRendMesh::InitWithData(MeshData* pMeshData)
	{
		DxRender* pDxRender = (DxRender*)GetRender();
		DxRenderDevice* pDxRenderDevice = (DxRenderDevice*)GetRenderDevice();

		int nTotalLod;
		m_ppD3DMesh = CreateD3DMesh( pDxRenderDevice->GetDXDevive(), pMeshData, &nTotalLod );

		m_pMeshData = pMeshData;

		return true;
	}

	void DxRendMesh::GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3 &vMax)
	{
		if (m_pMeshData)
		{
			m_pMeshData->GetBoundingAABB(&vMin,&vMax);
		}
	}
}
