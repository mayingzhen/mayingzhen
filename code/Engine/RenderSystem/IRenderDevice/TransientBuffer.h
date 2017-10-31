#pragma once



namespace ma
{

class VertexBuffer;
class IndexBuffer;
class HardwareBuffer;


struct SubAllocVB
{
	BYTE* m_pVertices;
	uint32 m_nAllocVerts;
	uint16	m_nFirstVertex;

	SubAllocVB()
	{
		m_pVertices = NULL;
		m_nAllocVerts = 0;
		m_nFirstVertex = 0;
	}
};

struct SubAllocIB
{
	uint16* m_pIndices;
	uint32 m_nAllocInds;
	uint32	m_nFirstIndex;

	SubAllocIB()
	{
		m_pIndices = 0;
		m_nAllocInds = 0;
		m_nFirstIndex = 0;
	}
};


class ParallHardWareBuffer : public Referenced 
{

public:
	ParallHardWareBuffer(uint32 nVertexStride,uint32 nNumVertice,uint32 numIndexes);
	~ParallHardWareBuffer();

	void LockVideoMemory();
	void UnLockVideoMemory();

	SubAllocVB AllocVertexBuffer(int nAllocVerts);
	SubAllocIB AllocIndexBuffer(int nAllocInds);

	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
	
	BYTE* GetVertexVideoMemoryBase() {return m_pVertexVideoMemoryBase;}
	BYTE* GetIndexVideoMemoryBase() {return m_pIndexVideoMemoryBase;}

private:

	// date for writing directly to VMEM
	RefPtr<VertexBuffer> m_pVertexBuffer;
	RefPtr<IndexBuffer> m_pIndexBuffer;

	// During writing, we only expose the base Video Memory pointer
	// and the offsets to the next free memory in this buffer
	BYTE* m_pVertexVideoMemoryBase;
	BYTE* m_pIndexVideoMemoryBase;

	std::atomic<uint32> m_nVertexOffset;
	std::atomic<uint32> m_nIndexOffset;

	// total amount of allocated memory for vertex/index buffers
	uint32 m_nVertexBufferAvailableMemory;
	uint32 m_nIndexBufferAvailableMemory;
	
	bool m_bLocked;
};


}