#include "MeshBatch.h"
#include "../Material/Material.h"

namespace ma
{

	MeshBatch::MeshBatch(uint32_t nStreanmStride,
						 PRIMITIVE_TYPE primitiveType,
						 bool indexed, uint32_t initialCapacity, 
						 uint32_t growSize)
	{
		m_nStreanmStride = nStreanmStride;

 		m_bIndexed = indexed;
 		m_nCapacity = initialCapacity;
		m_nVertexCapacity = initialCapacity;
		m_nIndexCapacity = initialCapacity;
 		m_nGrowSize = growSize;
		m_pVerticesPtr = NULL;
		m_pVerticesPtrBase = NULL;
		m_pIndicesPtr = NULL;
		m_pIndicesPtrBase = NULL;
	 
 		m_ePrimitiveType = primitiveType;

		m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(NULL,m_nIndexCapacity * sizeof(uint16_t),sizeof(uint16_t),HBU_DYNAMIC);
		m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer(NULL, m_nVertexCapacity * m_nStreanmStride,
			m_nStreanmStride,HBU_DYNAMIC);

		m_pSubMeshData = new SubMeshData();
	}

	MeshBatch::~MeshBatch()
	{
	}

	void MeshBatch::Render(Technique* pTech)
	{
		if (m_pVertexBuffer)
		{
			m_pVertexBuffer->Unlock();
		}

		if (m_pIndexBuffer)
		{
			m_pIndexBuffer->Unlock();
		}

		pTech->Bind(this);

		//GetRenderSystem()->DrawRenderable(this,pTech);
	}

	void MeshBatch::Add(const void* vertices, size_t size, uint32_t vertexCount, const uint16_t* indices, uint32_t indexCount)
	{
		ASSERT(vertices);

		uint32_t newVertexCount = m_pSubMeshData->m_nVertexCount + vertexCount;
		uint32_t newIndexCount = m_pSubMeshData->m_nIndexCount + indexCount;
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
		uint32_t vBytes = vertexCount * m_nStreanmStride;
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


	bool MeshBatch::Resize(uint32_t capacity)
	{
		if (capacity == 0)
		{
			LogError("Invalid resize capacity (0).");
			return false;
		}

		if (capacity <= m_nCapacity)
			return true;

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
		uint32_t indexCapacity = vertexCapacity;
		if (m_bIndexed && indexCapacity > USHRT_MAX)
		{
			LogError("Index capacity is greater than the maximum unsigned short value (%d > %d).", indexCapacity, USHRT_MAX);
			return false;
		}


		// Assign new capacities
		m_nCapacity = capacity;
		m_nVertexCapacity = vertexCapacity;
		m_nIndexCapacity = indexCapacity;

		// Update our vertex attribute bindings now that our client array pointers have changed
		//UpdateVB IB
		if (m_bIndexed)
		{
			uint32_t ioffset = m_pIndicesPtr - m_pIndicesPtrBase;

			vector<uint16_t> oldIndices;
			oldIndices.resize(ioffset / sizeof(uint16_t));
			memcpy(&oldIndices[0],m_pIndicesPtrBase,ioffset);

			m_pIndexBuffer->Unlock();
			m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(NULL,indexCapacity * sizeof(uint16_t),sizeof(uint16_t),HBU_DYNAMIC);
			m_pIndicesPtrBase = (uint16_t*)m_pIndexBuffer->Lock(LOCK_DISCARD);
			memcpy(m_pIndicesPtrBase,&oldIndices[0],ioffset);
			m_pIndicesPtr = m_pIndicesPtrBase + ioffset;
		}

		uint32_t voffset = m_pVerticesPtr - m_pVerticesPtrBase;

		vector<uint8_t> oldVertices;
		oldVertices.resize(voffset);
		memcpy(&oldVertices[0],m_pVerticesPtrBase,voffset);

		m_pVertexBuffer->Unlock();
		m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer(NULL, vertexCapacity * m_nStreanmStride, m_nStreanmStride,HBU_DYNAMIC);
		m_pVerticesPtrBase = (uint8_t*)m_pVertexBuffer->Lock(LOCK_DISCARD);
		memcpy(m_pVerticesPtrBase, &oldVertices[0], voffset);
		m_pVerticesPtr = m_pVerticesPtrBase + voffset;

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

		m_pVerticesPtrBase  = m_pVertexBuffer ? (uint8_t*)m_pVertexBuffer->Lock(LOCK_DISCARD) : NULL;
		m_pVerticesPtr = m_pVerticesPtrBase;

		m_pIndicesPtrBase  = m_pIndexBuffer ? (uint16_t*)m_pIndexBuffer->Lock(LOCK_DISCARD) : NULL;
		m_pIndicesPtr = m_pIndicesPtrBase;
	}
}
