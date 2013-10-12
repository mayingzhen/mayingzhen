#include "MeshBatch.h"
#include "Material/Material.h"
#include "RenderThread.h"

namespace ma
{

	MeshBatch::MeshBatch(VertexDeclaration* vertexFormat, 
						 PRIMITIVE_TYPE primitiveType, Effect* pEffect, 
						 bool indexed, UINT initialCapacity, 
						 UINT growSize)
	{
 		m_bIndexed = indexed;
 		m_nCapacity = 0;
		m_nVertexCapacity = 0;
		m_nIndexCapacity = 0;
 		m_nGrowSize = growSize;
		m_pVerticesPtr = NULL;
		m_pIndicesPtr = NULL;
	 
	 
 		m_pDeclaration = vertexFormat;
 		m_ePrimitiveType = primitiveType;
 		m_pMaterial = new Material();
		m_pMaterial->SetEffect(pEffect);

		resize(initialCapacity);

		m_pSubMeshData = new SubMeshData();
	}

	MeshBatch::~MeshBatch()
	{
		SAFE_DELETE(m_pMaterial);
		SAFE_DELETE(m_pSubMeshData);
	}

	void MeshBatch::add(const void* vertices, size_t size, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
	{
		SubMeshData* pSubMeshData = m_pSubMeshData;

		ASSERT(vertices);

		unsigned int newVertexCount = pSubMeshData->m_nVertexCount + vertexCount;
		unsigned int newIndexCount = pSubMeshData->m_nIndexCount + indexCount;
		if (m_ePrimitiveType == PRIM_TRIANGLESTRIP && pSubMeshData->m_nVertexCount > 0)
			newIndexCount += 2; // need an extra 2 indices for connecting strips with degenerate triangles
	    
		// Do we need to grow the batch?
		while (newVertexCount > m_nVertexCapacity || (m_bIndexed && newIndexCount > m_nIndexCapacity))
		{
			if (m_nGrowSize == 0)
				return; // growing disabled, just clip batch

			if (!resize(m_nCapacity + m_nGrowSize))
				return; // failed to grow

		}
	    
		// Copy vertex data.
		ASSERT(m_pVerticesPtr);
		unsigned int vBytes = vertexCount * m_pDeclaration->GetStreanmStride();
		memcpy(m_pVerticesPtr, vertices, vBytes);

	    
		// Copy index data.
		if (m_bIndexed)
		{
			ASSERT(indices);
			ASSERT(m_pIndicesPtr);

			if (pSubMeshData->m_nVertexCount == 0)
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
					m_pIndicesPtr[1] = pSubMeshData->m_nVertexCount;
					m_pIndicesPtr += 2;
				}
	            
				// Loop through all indices and insert them, with their values offset by
				// 'vertexCount' so that they are relative to the first newly inserted vertex.
				for (unsigned int i = 0; i < indexCount; ++i)
				{
					m_pIndicesPtr[i] = indices[i] + pSubMeshData->m_nVertexCount;
				}
			}

			m_pIndicesPtr += indexCount;
			pSubMeshData->m_nIndexCount = newIndexCount;
		}

		m_pVerticesPtr += vBytes;
		pSubMeshData->m_nVertexCount = newVertexCount;
	    
	}

// 	unsigned int MeshBatch::getCapacity() const
// 	{
// 		return m_nCapacity;
// 	}

// 	void MeshBatch::setCapacity(UINT capacity)
// 	{
// 		resize(capacity);
// 	}

	bool MeshBatch::resize(unsigned int capacity)
	{
		int index = GetRenderThread()->m_nCurThreadProcess;

		if (capacity == 0)
		{
			GP_ERROR("Invalid resize capacity (0).");
			return false;
		}

		if (capacity == m_nCapacity)
			return true;

		// Store old batch data.
		unsigned char* oldVertices = m_pVertexBuffers ? (unsigned char*)m_pVertexBuffers->GetData() : NULL;
		unsigned short* oldIndices = m_pIndexBuffer ? (unsigned short*)m_pIndexBuffer->GetData() : NULL;

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
			GP_ERROR("Unsupported primitive type for mesh batch (%d).", m_ePrimitiveType);
			return false;
		}

		// We have no way of knowing how many vertices will be stored in the batch
		// (we only know how many indices will be stored). Assume the worst case
		// for now, which is the same number of vertices as indices.
		unsigned int indexCapacity = vertexCapacity;
		if (m_bIndexed && indexCapacity > USHRT_MAX)
		{
			GP_ERROR("Index capacity is greater than the maximum unsigned short value (%d > %d).", indexCapacity, USHRT_MAX);
			return false;
		}

		
		// Allocate new data and reset pointers.
		UINT voffset = m_pVerticesPtr - oldVertices;
		UINT vBytes = vertexCapacity * m_pDeclaration->GetStreanmStride();
		unsigned char* newVertices = new unsigned char[vBytes];
		memset(newVertices, 0, vBytes);
		if (voffset >= vBytes)
			voffset = vBytes - 1;
		m_pVerticesPtr = newVertices + voffset;

		unsigned short* newIndices = NULL;
		if (m_bIndexed)
		{
			unsigned int ioffset = m_pIndicesPtr - oldIndices;
			newIndices = new unsigned short[indexCapacity];
			memset(newIndices, 0, indexCapacity * sizeof(unsigned short) );
			if (ioffset >= indexCapacity)
				ioffset = indexCapacity - 1;
			m_pIndicesPtr = newIndices + ioffset;
		}

		// Copy old data back in
		if (oldVertices)
			memcpy(newVertices, oldVertices, Min(m_nVertexCapacity, vertexCapacity) * m_pDeclaration->GetStreanmStride());
		SAFE_DELETE_ARRAY(oldVertices);

		if (oldIndices)
			memcpy(newIndices, oldIndices, Min(m_nIndexCapacity, indexCapacity) * sizeof(unsigned short));
		SAFE_DELETE_ARRAY(oldIndices);

		// Assign new capacities
		m_nCapacity = capacity;
		m_nVertexCapacity = vertexCapacity;
		m_nIndexCapacity = indexCapacity;

		// Update our vertex attribute bindings now that our client array pointers have changed
		//updateVertexAttributeBinding();
		//UpdateVB IB
		if (m_bIndexed)
		{
			SAFE_DELETE(m_pIndexBuffer);
			m_pIndexBuffer = GetRenderDevice()->CreateIndexBuffer(newIndices,indexCapacity * sizeof(unsigned short),INDEX_TYPE_U16,USAGE_DYNAMIC);
		}

		SAFE_DELETE(m_pVertexBuffers);
		m_pVertexBuffers = GetRenderDevice()->CreateVertexBuffer(newVertices, vBytes,m_pDeclaration->GetStreanmStride(),USAGE_DYNAMIC);

		return true;
	}

	void MeshBatch::add(const float* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount)
	{
		add(vertices, sizeof(float), vertexCount, indices, indexCount);
	}
	    
	void MeshBatch::start()
	{
		m_pSubMeshData->m_nVertexCount = 0;
		m_pSubMeshData->m_nIndexCount = 0;
		m_pVerticesPtr = m_pVertexBuffers ? (unsigned char*)m_pVertexBuffers->GetData() : NULL;
		m_pIndicesPtr = m_pIndexBuffer ? (unsigned short*)m_pIndexBuffer->GetData() : NULL;
	}

	void MeshBatch::finish()
	{
		GetRenderSystem()->DrawDyRenderable(this);
	}
}
