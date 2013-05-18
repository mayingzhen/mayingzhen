#include "RendMesh.h"
#include "VertexStream.h"

namespace ma
{


	bool RenderMesh::InitWithData(MeshData* pMeshData)
	{
		if (pMeshData == NULL)
			return false;

		m_pMesData = pMeshData;

		IndexBuffer* pIndexBuffer = GetRenderDevice()->CreateIndexBuffer();
		INDEX_TYPE eIndexType = pMeshData->GetIndexType() == INDEX_TYPE_U16 ? INDEX_TYPE_U16 : INDEX_TYPE_U32; 
		pIndexBuffer->Bind( pMeshData->GetIndexBuffer(), pMeshData->GetIndexBufferSize(), eIndexType );

		VertexBuffer* pVertexBuffer = GetRenderDevice()->CreateVertexBuffer();
		pVertexBuffer->Bind( pMeshData->GetVertexBuffer(), pMeshData->GetVertexBufferSize(), pMeshData->GetVertexType() );
		
		VertexDeclaration* pDeclaration = GetRenderDevice()->CreateVertexDeclaration();	
		pDeclaration->Init( pMeshData->GetVertexType() );

		m_ePrimitiveType = PRIM_TRIANGLELIST;
		m_pIndexBuffer = pIndexBuffer;
		m_pDeclaration = pDeclaration;
		m_pVertexBuffers = pVertexBuffer;

		return true;
	}



// 	void RenderMesh::Draw()
// 	{
// 		if (GetRenderDevice() == NULL)
// 			return;
// 
// 		GetRenderDevice()->SetVertexStream(m_pVertexStream);
// 		PRIMITIVE_TYPE type = m_pVertexStream->m_ePrimitiveType;
// 
// 		for (UINT i = 0; i < m_pMesData->GetSubMeshNumber(); ++i)
// 		{
// 			SubMeshData* pSubMesh = m_pMesData->GetSubMeshByIndex(i);
// 			if (pSubMesh == NULL)
// 				continue;
// 
// 			GetRenderDevice()->DrawIndexedPrimitive(type,pSubMesh->m_nVertexStart,pSubMesh->m_nVertexCount,
// 				pSubMesh->m_nIndexStart,pSubMesh->m_nIndexCount);
// 		}
// 	}
}

