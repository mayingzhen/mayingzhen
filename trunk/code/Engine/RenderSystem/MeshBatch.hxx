//#include "Base.h"
#include "MeshBatch.h"
#include "Material/Material.h"
#include "RenderThread.h"

namespace ma
{

	MeshBatch::MeshBatch(VertexDeclaration* vertexFormat, 
						 PRIMITIVE_TYPE primitiveType, Material* material, 
						 bool indexed, unsigned int initialCapacity, 
						 unsigned int growSize)
	{
 		_indexed = indexed;
 		_capacity = 0;
		_vertexCapacity = 0;
		_indexCapacity = 0;
 		_growSize = growSize;
		_verticesPtr = NULL;
		_indicesPtr = NULL;
	 
	 
 		m_pDeclaration = vertexFormat;
 		m_ePrimitiveType = primitiveType;
 		m_pMaterial = material;

		resize(initialCapacity);

		m_pSubMeshData = new SubMeshData();
	}

	MeshBatch::~MeshBatch()
	{
		//SAFE_RELEASE(_material);
		//SAFE_DELETE_ARRAY(_vertices);
		//SAFE_DELETE_ARRAY(_indices);

		SAFE_DELETE(m_pSubMeshData);
	}

	MeshBatch* MeshBatch::create(VertexDeclaration* vertexFormat, PRIMITIVE_TYPE primitiveType, const char* materialPath, bool indexed, unsigned int initialCapacity, unsigned int growSize)
	{
		ASSERT(false);
		return NULL;
// 		Material* material = Material::create(materialPath);
// 		if (material == NULL)
// 		{
// 			GP_ERROR("Failed to create material for mesh batch from file '%s'.", materialPath);
// 			return NULL;
// 		}
// 		MeshBatch* batch = create(vertexFormat, primitiveType, material, indexed, initialCapacity, growSize);
// 		//SAFE_RELEASE(material); // batch now owns the material
// 		return batch;
	}

	MeshBatch* MeshBatch::create(VertexDeclaration* vertexFormat, PRIMITIVE_TYPE primitiveType, Material* material, bool indexed, unsigned int initialCapacity, unsigned int growSize)
	{
		ASSERT(material);

		MeshBatch* batch = new MeshBatch(vertexFormat, primitiveType, material, indexed, initialCapacity, growSize);

		//material->IncReference();

		return batch;
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
		while (newVertexCount > _vertexCapacity || (_indexed && newIndexCount > _indexCapacity))
		{
			if (_growSize == 0)
				return; // growing disabled, just clip batch

			if (!resize(_capacity + _growSize))
				return; // failed to grow

		}
	    
		// Copy vertex data.
		ASSERT(_verticesPtr);
		unsigned int vBytes = vertexCount * m_pDeclaration->GetStreanmStride();
		memcpy(_verticesPtr, vertices, vBytes);


	    
		// Copy index data.
		if (_indexed)
		{
			ASSERT(indices);
			ASSERT(_indicesPtr);

			if (pSubMeshData->m_nVertexCount == 0)
			{
				// Simply copy values directly into the start of the index array.
				memcpy(_indicesPtr, indices, indexCount * sizeof(unsigned short));
			}
			else
			{
				if (m_ePrimitiveType == PRIM_TRIANGLESTRIP)
				{
					// Create a degenerate triangle to connect separate triangle strips
					// by duplicating the previous and next vertices.
					_indicesPtr[0] = *(_indicesPtr - 1);
					_indicesPtr[1] = pSubMeshData->m_nVertexCount;
					_indicesPtr += 2;
				}
	            
				// Loop through all indices and insert them, with their values offset by
				// 'vertexCount' so that they are relative to the first newly inserted vertex.
				for (unsigned int i = 0; i < indexCount; ++i)
				{
					_indicesPtr[i] = indices[i] + pSubMeshData->m_nVertexCount;
				}
			}

			_indicesPtr += indexCount;
			pSubMeshData->m_nIndexCount = newIndexCount;
		}

		_verticesPtr += vBytes;
		pSubMeshData->m_nVertexCount = newVertexCount;
	    
	}

// 	unsigned int MeshBatch::getCapacity() const
// 	{
// 		return _capacity;
// 	}

	void MeshBatch::setCapacity(UINT capacity)
	{
		resize(capacity);
	}

	bool MeshBatch::resize(unsigned int capacity)
	{
		int index = GetRenderThread()->m_nCurThreadProcess;

		if (capacity == 0)
		{
			GP_ERROR("Invalid resize capacity (0).");
			return false;
		}

		if (capacity == _capacity)
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
		if (_indexed && indexCapacity > USHRT_MAX)
		{
			GP_ERROR("Index capacity is greater than the maximum unsigned short value (%d > %d).", indexCapacity, USHRT_MAX);
			return false;
		}

		
		// Allocate new data and reset pointers.
		UINT voffset = _verticesPtr - oldVertices;
		UINT vBytes = vertexCapacity * m_pDeclaration->GetStreanmStride();
		unsigned char* newVertices = new unsigned char[vBytes];
		memset(newVertices, 0, vBytes);
		if (voffset >= vBytes)
			voffset = vBytes - 1;
		_verticesPtr = newVertices + voffset;

		unsigned short* newIndices = NULL;
		if (_indexed)
		{
			unsigned int ioffset = _indicesPtr - oldIndices;
			newIndices = new unsigned short[indexCapacity];
			memset(newIndices, 0, indexCapacity * sizeof(unsigned short) );
			if (ioffset >= indexCapacity)
				ioffset = indexCapacity - 1;
			_indicesPtr = newIndices + ioffset;
		}

		// Copy old data back in
		if (oldVertices)
			memcpy(newVertices, oldVertices, Min(_vertexCapacity, vertexCapacity) * m_pDeclaration->GetStreanmStride());
		SAFE_DELETE_ARRAY(oldVertices);

		if (oldIndices)
			memcpy(newIndices, oldIndices, Min(_indexCapacity, indexCapacity) * sizeof(unsigned short));
		SAFE_DELETE_ARRAY(oldIndices);

		// Assign new capacities
		_capacity = capacity;
		_vertexCapacity = vertexCapacity;
		_indexCapacity = indexCapacity;

		// Update our vertex attribute bindings now that our client array pointers have changed
		//updateVertexAttributeBinding();
		//UpdateVB IB
		if (_indexed)
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
		_verticesPtr = m_pVertexBuffers ? (unsigned char*)m_pVertexBuffers->GetData() : NULL;
		_indicesPtr = m_pIndexBuffer ? (unsigned short*)m_pIndexBuffer->GetData() : NULL;
	}

	void MeshBatch::finish()
	{
// 		if (_indices)
// 		{
// 			void* pIbLock = m_pIndexBuffer->Lock(0,m_pIndexBuffer->GetSize(),LOCK_DISCARD);		
// 			memcpy(pIbLock,_indices,_indexCount * sizeof(Uint16));	
// 			m_pIndexBuffer->Unlock();
// 		}
// 
// 		void* pVbLock = m_pVertexBuffers->Lock(0,m_pVertexBuffers->GetSize(),LOCK_DISCARD);
// 		memcpy(pVbLock,_vertices,_vertexCount * m_pDeclaration->GetStreanmStride());	
// 		m_pVertexBuffers->Unlock();
// 
 		//m_pSubMeshData->m_nIndexCount = _indexCount;
 		//m_pSubMeshData->m_nVertexCount = _vertexCount;

		GetRenderSystem()->DrawDyRenderable(this);
	}

// 	void MeshBatch::draw()
// 	{
// 		GetRenderSystem()->DrawDyRenderable(this);
// 	}
     

}
