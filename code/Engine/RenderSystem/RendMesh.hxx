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
		IndexBuffer* pIndexBuffer = GetRenderDevice()->CreateIndexBuffer(pIndexData,nIndexSize,eIndexType);
		pIndexBuffer->Active();

		void* pVertexData = pMeshData->GetVertexBuffer();
		int nVertexDataSize = pMeshData->GetVertexBufferSize();
		int nVertexStride = pMeshData->GetVertexStride();
		VertexBuffer* pVertexBuffer = GetRenderDevice()->CreateVertexBuffer(pVertexData,nVertexDataSize, nVertexStride);
		pVertexBuffer->Active();
		
		VertexDeclaration* pDeclaration = GetRenderDevice()->CreateVertexDeclaration();	
		pDeclaration->Init( pMeshData->GetVertexType() );
		pDeclaration->Active();

		//Renderable* pRenderable = new Renderable();

		for (UINT i = 0; i < pMeshData->GetSubMeshNumber(); ++i)
		{
			SubMeshData* pSubMesh = pMeshData->GetSubMeshByIndex(i);
			ASSERT(pSubMesh);
			if (pSubMesh == NULL)
				continue;

			Renderable* pRenderable = new Renderable();
			pRenderable->m_ePrimitiveType = PRIM_TRIANGLELIST;
			pRenderable->m_pDeclaration = pDeclaration;
			pRenderable->m_pVertexBuffers = pVertexBuffer;
			pRenderable->m_pIndexBuffer = pIndexBuffer;
			pRenderable->m_nIndexCount = pSubMesh->m_nIndexCount;
			pRenderable->m_nIndexStart = pSubMesh->m_nIndexStart;
			pRenderable->m_nVertexStart = pSubMesh->m_nVertexStart;
			pRenderable->m_nVertexCount = pSubMesh->m_nVertexCount;

			m_arrRenderable.push_back(pRenderable);
		}

		return true;
	}

	void RenderMesh::SetMaterial(Material* pMaterial)
	{
		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			m_arrRenderable[i]->m_pMaterial = pMaterial;
		}
	}

	void RenderMesh::Draw()
	{
		for (UINT i = 0; i < m_arrRenderable.size(); ++i)
		{
			GetRenderDevice()->DrawRenderable(m_arrRenderable[i]);
		}
	}
}

