#include "TransientBuffer.h"
#include "../../Time/Timer.h"

namespace ma
{

ParallHardWareBuffer::ParallHardWareBuffer(uint32 nVertexStride,uint32 nNumVertice,uint32 numIndexes)
{
	m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer(NULL,nVertexStride * nNumVertice,nVertexStride,HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	m_pVertexVideoMemoryBase = NULL;
	m_nVertexOffset = 0;	
	m_nVertexBufferAvailableMemory = nVertexStride * nNumVertice;
	
	INDEX_TYPE idxType = INDEX_TYPE_U16;
	if (numIndexes > 0)
	{
		m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(NULL,sizeof(uint16) * numIndexes,sizeof(uint16),HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	}
	m_pIndexVideoMemoryBase = NULL;
	m_nIndexOffset = 0;
	m_nIndexBufferAvailableMemory = sizeof(uint16) * numIndexes;
}

ParallHardWareBuffer::~ParallHardWareBuffer()
{

}

void ParallHardWareBuffer::LockVideoMemory()
{
	// lock video memory vertex/index buffer and expose base pointer and offset
	if(m_pVertexBuffer)
		m_pVertexVideoMemoryBase = (BYTE*)m_pVertexBuffer->Lock(LOCK_DISCARD);

	if(m_pIndexBuffer)
		m_pIndexVideoMemoryBase = (BYTE*)m_pIndexBuffer->Lock(LOCK_DISCARD);

	m_nVertexOffset = 0;
	m_nIndexOffset = 0;	
}

void ParallHardWareBuffer::UnLockVideoMemory()
{
	// unlock vertex/index buffer
	if(m_pVertexBuffer)
		m_pVertexBuffer->Unlock();

	if(m_pIndexBuffer)
		m_pIndexBuffer->Unlock();
}



SubAllocVB ParallHardWareBuffer::AllocVertexBuffer(int nAllocVerts)
{
	SubAllocVB subAlloc;

	uint32 nVertexStride = m_pVertexBuffer->GetStride();

	// Reserve vertex memory, thread-safe
	BYTE* pVertices				  = m_pVertexVideoMemoryBase;
	uint32 nAvailableVertexMemory = m_nVertexBufferAvailableMemory;

	// Limit vertex count for 16-bit indices.
	//if (nAllocInds > 0)
		nAllocVerts = min(nAllocVerts, (1<<16));

	LONG nVerticesOffset;
	do {
		nVerticesOffset = *(volatile LONG*)&m_nVertexOffset;
		if (nVerticesOffset + nAllocVerts * nVertexStride > nAvailableVertexMemory)
		{
			ASSERT(false);
			nAllocVerts = (nAvailableVertexMemory - nVerticesOffset) / nVertexStride;
		}
	} while (InterlockedCompareExchange( (volatile LONG*)&m_nVertexOffset, nVerticesOffset + nAllocVerts * nVertexStride, nVerticesOffset ) != nVerticesOffset );

	subAlloc.m_pVertices = pVertices + nVerticesOffset; 
	subAlloc.m_nAllocVerts = nAllocVerts;
	subAlloc.m_nFirstVertex = static_cast<uint16>(nVerticesOffset / nVertexStride);

	return subAlloc;
}

SubAllocIB ParallHardWareBuffer::AllocIndexBuffer(int nAllocInds)
{
	SubAllocIB subAlloc;

	ASSERT(m_pIndexBuffer);
	if (m_pIndexBuffer == NULL)
		return subAlloc;

	// Reserve index memory, thread-safe
	BYTE* pIndices							= m_pIndexVideoMemoryBase;
	uint32 nAvailableIndexMemory			= m_nIndexBufferAvailableMemory;

	LONG nIndicesOffset;
	do {
		nIndicesOffset = *(volatile LONG*)&m_nIndexOffset;
		if (nIndicesOffset + nAllocInds * sizeof(uint16) > nAvailableIndexMemory)
		{
			ASSERT(false);
			nAllocInds = (nAvailableIndexMemory - nIndicesOffset) / sizeof(uint16);
		}
	} while (InterlockedCompareExchange( (volatile LONG*)&m_nIndexOffset, nIndicesOffset + nAllocInds * sizeof(uint16), nIndicesOffset ) != nIndicesOffset );

	subAlloc.m_pIndices = (uint16*)(pIndices + nIndicesOffset);
	subAlloc.m_nAllocInds = nAllocInds;
	subAlloc.m_nFirstIndex = nIndicesOffset / sizeof(uint16);

	return subAlloc;
}

}