#include "TransientBuffer.h"
#include "../../Time/Timer.h"

namespace ma
{

ParallHardWareBuffer::ParallHardWareBuffer(uint32_t nVertexStride,uint32_t nNumVertice,uint32_t numIndexes)
{
	m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer(NULL,nVertexStride * nNumVertice,nVertexStride,HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	m_pVertexVideoMemoryBase = NULL;
	m_nVertexOffset = 0;	
	m_nVertexBufferAvailableMemory = nVertexStride * nNumVertice;
	
	if (numIndexes > 0)
	{
		m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer(NULL,sizeof(uint16_t) * numIndexes,sizeof(uint16_t),HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	}
	m_pIndexVideoMemoryBase = NULL;
	m_nIndexOffset = 0;
	m_nIndexBufferAvailableMemory = sizeof(uint16_t) * numIndexes;

	m_bLocked = false;
}

ParallHardWareBuffer::~ParallHardWareBuffer()
{

}

VertexBuffer* ParallHardWareBuffer::GetVertexBuffer() 
{
	return m_pVertexBuffer.get();
}

IndexBuffer* ParallHardWareBuffer::GetIndexBuffer() 
{
	return m_pIndexBuffer.get();
}

void ParallHardWareBuffer::LockVideoMemory()
{
	// lock video memory vertex/index buffer and expose base pointer and offset
	if(m_pVertexBuffer)
		m_pVertexVideoMemoryBase = (uint8_t*)m_pVertexBuffer->Lock(LOCK_DISCARD);

	if(m_pIndexBuffer)
		m_pIndexVideoMemoryBase = (uint8_t*)m_pIndexBuffer->Lock(LOCK_DISCARD);

	m_nVertexOffset = 0;
	m_nIndexOffset = 0;	

	m_bLocked = true;
}

void ParallHardWareBuffer::UnLockVideoMemory()
{
	// unlock vertex/index buffer
	if(m_pVertexBuffer)
		m_pVertexBuffer->Unlock();

	if(m_pIndexBuffer)
		m_pIndexBuffer->Unlock();

	m_pVertexVideoMemoryBase = NULL;
	m_pIndexVideoMemoryBase = NULL;

	m_bLocked = false;
}



SubAllocVB ParallHardWareBuffer::AllocVertexBuffer(int nAllocVerts)
{
	ASSERT(m_bLocked);

	SubAllocVB subAlloc;

	uint32_t nVertexStride = m_pVertexBuffer->GetStride();

	// Reserve vertex memory, thread-safe
	uint8_t* pVertices				  = m_pVertexVideoMemoryBase;
	uint32_t nAvailableVertexMemory = m_nVertexBufferAvailableMemory;

	// Limit vertex count for 16-bit indices.
	//if (nAllocInds > 0)
		nAllocVerts = min(nAllocVerts, (1<<16));

	uint32_t nVerticesOffset;
	do {
		nVerticesOffset = m_nVertexOffset.load();
		if (nAllocVerts * nVertexStride > nAvailableVertexMemory)
		{
			ASSERT(false);
			nAllocVerts = nAvailableVertexMemory / nVertexStride;
		}
	} while ( !m_nVertexOffset.compare_exchange_strong(nVerticesOffset, nVerticesOffset + nAllocVerts * nVertexStride) );

	subAlloc.m_pVertices = pVertices + nVerticesOffset; 
	subAlloc.m_nAllocVerts = nAllocVerts;
	subAlloc.m_nFirstVertex = static_cast<uint16_t>(nVerticesOffset / nVertexStride);

	//ASSERT(subAlloc.m_nFirstVertex < 1000);

	return subAlloc;
}

SubAllocIB ParallHardWareBuffer::AllocIndexBuffer(int nAllocInds)
{
	ASSERT(m_bLocked);

	SubAllocIB subAlloc;

	ASSERT(m_pIndexBuffer);
	if (m_pIndexBuffer == NULL)
		return subAlloc;

	// Reserve index memory, thread-safe
	uint8_t* pIndices							= m_pIndexVideoMemoryBase;
	uint32_t nAvailableIndexMemory			= m_nIndexBufferAvailableMemory;

	uint32_t nIndicesOffset;
	do {
		nIndicesOffset = m_nIndexOffset.load();
		if (nAllocInds * sizeof(uint16_t) > nAvailableIndexMemory)
		{
			ASSERT(false);
			nAllocInds = nAvailableIndexMemory / sizeof(uint16_t);
		}
	}
	while ( !m_nIndexOffset.compare_exchange_strong(nIndicesOffset, nIndicesOffset + nAllocInds * sizeof(uint16_t) ) );

	subAlloc.m_pIndices = (uint16_t*)(pIndices + nIndicesOffset);
	subAlloc.m_nAllocInds = nAllocInds;
	subAlloc.m_nFirstIndex = nIndicesOffset / sizeof(uint16_t);

	return subAlloc;
}

}
