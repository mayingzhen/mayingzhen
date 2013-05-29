#include "RendMesh.h"

namespace ma
{


	bool RenderMesh::InitWithData(MeshData* pMeshData)
	{
		if (pMeshData == NULL)
			return false;

		m_pMesData = pMeshData;

		void* pIndexData = pMeshData->GetIndexBuffer();
		int nIndexSize = pMeshData->GetIndexBufferSize();
		INDEX_TYPE eIndexType = pMeshData->GetIndexType() == INDEX_TYPE_U16 ? INDEX_TYPE_U16 : INDEX_TYPE_U32; 
		IndexBuffer* pIndexBuffer = GetRenderDevice()->CreateIndexBuffer(pIndexBuffer,nIndexSize,eIndexType);
		pIndexBuffer->Active();

		void* pVertexData = pMeshData->GetVertexBuffer();
		int nVertexDataSize = pMeshData->GetVertexBufferSize();
		int nVertexStride = pMeshData->GetVertexStride();
		VertexBuffer* pVertexBuffer = GetRenderDevice()->CreateVertexBuffer(pVertexData,nVertexDataSize, nVertexStride);
		pVertexBuffer->Active();
		
		VertexDeclaration* pDeclaration = GetRenderDevice()->CreateVertexDeclaration();	
		pDeclaration->Init( pMeshData->GetVertexType() );
		pDeclaration->Active();

		Renderable* pRenderable = new Renderable();

// 		for (UINT i = 0; i < pMeshData->GetSubMeshNumber(); ++i)
// 		{
// 			SubMeshData
// 		}

		pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
		pRenderable->m_pIndexBuffer = pIndexBuffer;
		pRenderable->m_pVertexBuffers = pVertexBuffer;
		pRenderable->m_pDeclaration = pDeclaration;

// 		m_ePrimitiveType = PRIM_TRIANGLELIST;
// 		m_pIndexBuffer = pIndexBuffer;
// 		m_pDeclaration = pDeclaration;
// 		m_pVertexBuffers = pVertexBuffer;

		return true;
	}



	void RenderMesh::Draw(Technique* pTechnique)
	{
		pTechnique->CommitChanges();

// 		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
// 		{
// 			GetRenderDevice()->DrawRenderable(m_arrRenderable[i]);
// 		}

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
	}
}

