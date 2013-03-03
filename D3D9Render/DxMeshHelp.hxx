#include "DxMeshHelp.h"

namespace ma
{
	template<class IndexType, class MeshDataType>
	void CopyIndexBuffer(void* destBuf, MeshDataType* pMeshData, const UINT nSubMeshNum, UINT numIndex ) 
	{
		IndexType* destIndexData = reinterpret_cast<IndexType*>(destBuf);
		const IndexType* arrInd = reinterpret_cast<const IndexType*>(pMeshData->GetIndexBuffer());
		UINT nIndexFilled = 0;
		for (UINT nSubMeshCnt = 0; nSubMeshCnt < nSubMeshNum; ++nSubMeshCnt)
		{
			SubMeshData* pSubMeshData = pMeshData->GetSubMesh(nSubMeshCnt);
			const UINT nVertexStart = pSubMeshData->GetVertexStart();
			const UINT nIndexCount =  pSubMeshData->GetIndexCount();
			for (UINT nIndCnt = 0; nIndCnt < nIndexCount; ++nIndCnt)
			{
				destIndexData[nIndexFilled] = nVertexStart + arrInd[nIndexFilled];
				++nIndexFilled;
			}
		}
		assert(nIndexFilled == numIndex); 
	}

	ID3DXMesh* CreateD3DMesh(IDirect3DDevice9* pd3dDevice,MeshData* pMeshData)
	{ 
		if (pd3dDevice == NULL)
			return NULL;

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
		bool bIsIndex32 = pMeshData->GetIsIndex32();
		UINT nIndexSize = bIsIndex32 ? sizeof(xmUint32) : sizeof(xmUint16);
		UINT numIndex = pMeshData->GetIndexNumber();
		UINT numVertex = pMeshData->GetVertexNumber();

		//Create D3DMesh
		const xmUint32 numFace = numIndex / 3;
		const UINT nSubMeshNum = pMeshData->GetSubMeshNumber();

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
			for (UINT nVCnt = 0; nVCnt < numVertex; ++nVCnt)
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


			for(UINT nSubMeshCnt = 0; nSubMeshCnt < nSubMeshNum; ++nSubMeshCnt)
			{
				SubMeshData* pSubMeshData = pMeshData->GetSubMesh(nSubMeshCnt);
				const UINT nTriNum = pSubMeshData->GetIndexCount() / 3;
				for (UINT nTriCnt = 0; nTriCnt < nTriNum; ++nTriCnt)
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

		return pD3DMesh;
	}
}

