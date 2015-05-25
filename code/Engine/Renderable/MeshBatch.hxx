#include "MeshBatch.h"
#include "../Material/Material.h"

namespace ma
{

	MeshBatch::MeshBatch(VertexDeclaration* vertexFormat, 
						 PRIMITIVE_TYPE primitiveType,
						 bool indexed, UINT initialCapacity, 
						 UINT growSize)
	{
 		m_bIndexed = indexed;
 		m_nCapacity = initialCapacity;
		m_nVertexCapacity = 0;
		m_nIndexCapacity = 0;
 		m_nGrowSize = growSize;
		m_pVerticesPtr = NULL;
		m_pIndicesPtr = NULL;
	 
 		m_pDeclaration = vertexFormat;
 		m_ePrimitiveType = primitiveType;

		if (m_pDeclaration)
		{
			Resize(initialCapacity);
		}

		m_pSubMeshData = new SubMeshData();
	}

	MeshBatch::~MeshBatch()
	{
	}

	void MeshBatch::Render(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		GetRenderSystem()->DrawDyRenderable(this,pTech);
	}

	void MeshBatch::Add(const void* vertices, size_t size, UINT vertexCount, const uint16* indices, UINT indexCount)
	{
		ASSERT(vertices);

		UINT newVertexCount = m_pSubMeshData->m_nVertexCount + vertexCount;
		UINT newIndexCount = m_pSubMeshData->m_nIndexCount + indexCount;
		if (m_ePrimitiveType == PRIM_TRIANGLESTRIP && m_pSubMeshData->m_nVertexCount > 0)
			newIndexCount += 2; // need an extra 2 indices for connecting strips with degenerate triangles
	    
		// Do we need to grow the batch?
		while (newVertexCount > m_nVertexCapacity || (m_bIndexed && newIndexCount > m_nIndexCapacity))
		{
			if (m_nGrowSize == 0)
				return; // growing disabled, just clip batch

			if (!Resize(m_nCapacity + m_nGrowSize))
				return; // failed to grow
	
		}
	    
		// Copy vertex data.
		ASSERT(m_pVerticesPtr);
		UINT vBytes = vertexCount * m_pDeclaration->GetStreanmStride();
		memcpy(m_pVerticesPtr, vertices, vBytes);

	    
		// Copy index data.
		if (m_bIndexed)
		{
			ASSERT(indices);
			ASSERT(m_pIndicesPtr);

			if (m_pSubMeshData->m_nVertexCount == 0)
			{
				// Simply copy values directly into the start of the index array.
				memcpy(m_pIndicesPtr, indices, indexCount * sizeof(unsigned short));
			}
			else
			{
				if (m_ePrimitiveType == PRIM_TRIANGLESTRIP)
				{
					// Create a degenerate triangle to connect separate triangle strips
					// by duplicating the previous and next vertices.
					m_pIndicesPtr[0] = *(m_pIndicesPtr - 1);
					m_pIndicesPtr[1] = m_pSubMeshData->m_nVertexCount;
					m_pIndicesPtr += 2;
				}
	            
				// Loop through all indices and insert them, with their values offset by
				// 'vertexCount' so that they are relative to the first newly inserted vertex.
				for (unsigned int i = 0; i < indexCount; ++i)
				{
					m_pIndicesPtr[i] = indices[i] + m_pSubMeshData->m_nVertexCount;
				}
			}

			m_pIndicesPtr += indexCount;
			m_pSubMeshData->m_nIndexCount = newIndexCount;
		}

		m_pVerticesPtr += vBytes;
		m_pSubMeshData->m_nVertexCount = newVertexCount;
	    
	}


	bool MeshBatch::Resize(UINT capacity)
	{
		if (capacity == 0)
		{
			LogError("Invalid resize capacity (0).");
			return false;
		}

		if (capacity == m_nCapacity)
			return true;

		// Store old batch data.
		uint8* oldVertices = m_pVertexBuffers ? (uint8*)m_pVertexBuffers->GetData() : NULL;
		uint16* oldIndices = m_pIndexBuffer ? (uint16*)m_pIndexBuffer->GetData() : NULL;

		unsigned int vertexCapacity = 0;
		switch (m_ePrimitiveType)
		{
		case PRIM_LINELIST:
			vertexCapacity = capacity * 2;
			break;
		case PRIM_LINESTRIP:
			vertexCapacity = capacity + 1;
			break;
		case PRIM_POINTLIST:
			vertexCapacity = capacity;
			break;
		case PRIM_TRIANGLELIST:
			vertexCapacity = capacity * 3;
			break;
		case PRIM_TRIANGLESTRIP:
			vertexCapacity = capacity + 2;
			break;
		default:
			LogError("Unsupported primitive type for mesh batch (%d).", m_ePrimitiveType);
			return false;
		}

		// We have no way of knowing how many vertices will be stored in the batch
		// (we only know how many indices will be stored). Assume the worst case
		// for now, which is the same number of vertices as indices.
		UINT indexCapacity = vertexCapacity;
		if (m_bIndexed && indexCapacity > USHRT_MAX)
		{
			LogError("Index capacity is greater than the maximum unsigned short value (%d > %d).", indexCapacity, USHRT_MAX);
			return false;
		}

		
		// Allocate new data and reset pointers.
		UINT voffset = m_pVerticesPtr - oldVertices;
		UINT vBytes = vertexCapacity * m_pDeclaration->GetStreanmStride();
		Byte* newVertices = new Byte[vBytes];
		memset(newVertices, 0, vBytes);
		if (voffset >= vBytes)
			voffset = vBytes - 1;
		m_pVerticesPtr = newVertices + voffset;

		uint16* newIndices = NULL;
		if (m_bIndexed)
		{
			UINT ioffset = m_pIndicesPtr - oldIndices;
			newIndices = new uint16[indexCapacity];
			memset(newIndices, 0, indexCapacity * sizeof(uint16) );
			if (ioffset >= indexCapacity)
				ioffset = indexCapacity - 1;
			m_pIndicesPtr = newIndices + ioffset;
		}

		// Copy old data back in
		if (oldVertices)
			memcpy(newVertices, oldVertices, Math::Min(m_nVertexCapacity, vertexCapacity) * m_pDeclaration->GetStreanmStride());
		SAFE_DELETE_ARRAY(oldVertices);

		if (oldIndices)
			memcpy(newIndices, oldIndices, Math::Min(m_nIndexCapacity, indexCapacity) * sizeof(uint16));
		SAFE_DELETE_ARRAY(oldIndices);

		// Assign new capacities
		m_nCapacity = capacity;
		m_nVertexCapacity = vertexCapacity;
		m_nIndexCapacity = indexCapacity;

		// Update our vertex attribute bindings now that our client array pointers have changed
		//UpdateVB IB
		if (m_bIndexed)
		{
			m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer((uint8*)newIndices,indexCapacity * sizeof(uint16),sizeof(uint16),USAGE_DYNAMIC);
		}

		m_pVertexBuffers = GetRenderSystem()->CreateVertexBuffer((uint8*)newVertices, vBytes,m_pDeclaration->GetStreanmStride(),USAGE_DYNAMIC);

		return true;
	}

	void MeshBatch::Add(const float* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
	{
		Add(vertices, sizeof(float), vertexCount, indices, indexCount);
	}
	    
	void MeshBatch::Reset()
	{
		m_pSubMeshData->m_nVertexStart = 0;
		m_pSubMeshData->m_nIndexStart = 0;
		m_pSubMeshData->m_nVertexCount = 0;
		m_pSubMeshData->m_nIndexCount = 0;
		m_pVerticesPtr = m_pVertexBuffers ? (uint8*)m_pVertexBuffers->GetData() : NULL;
		m_pIndicesPtr = m_pIndexBuffer ? (uint16*)m_pIndexBuffer->GetData() : NULL;
	}
}
