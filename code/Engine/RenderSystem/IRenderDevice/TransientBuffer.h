#pragma once



namespace ma
{

	class VertexBuffer;
	class IndexBuffer;
	class HardwareBuffer;


	struct SubAllocVB
	{
		uint8_t*	m_pVertices;
		uint32_t	m_nAllocVerts;
		uint16_t	m_nFirstVertex;

		SubAllocVB()
		{
			m_pVertices = NULL;
			m_nAllocVerts = 0;
			m_nFirstVertex = 0;
		}
	};

	struct SubAllocIB
	{
		uint16_t* m_pIndices;
		uint32_t m_nAllocInds;
		uint32_t	m_nFirstIndex;

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
		ParallHardWareBuffer(uint32_t nVertexStride,uint32_t nNumVertice,uint32_t numIndexes);
		~ParallHardWareBuffer();

		void LockVideoMemory();
		void UnLockVideoMemory();

		SubAllocVB AllocVertexBuffer(int nAllocVerts);
		SubAllocIB AllocIndexBuffer(int nAllocInds);

		VertexBuffer* GetVertexBuffer();
		IndexBuffer* GetIndexBuffer();
	
		uint8_t* GetVertexVideoMemoryBase() {return m_pVertexVideoMemoryBase;}
		uint8_t* GetIndexVideoMemoryBase() {return m_pIndexVideoMemoryBase;}

	private:

		// date for writing directly to VMEM
		RefPtr<VertexBuffer> m_pVertexBuffer;
		RefPtr<IndexBuffer> m_pIndexBuffer;

		// During writing, we only expose the base Video Memory pointer
		// and the offsets to the next free memory in this buffer
		uint8_t* m_pVertexVideoMemoryBase;
		uint8_t* m_pIndexVideoMemoryBase;

		std::atomic<uint32_t> m_nVertexOffset;
		std::atomic<uint32_t> m_nIndexOffset;

		// total amount of allocated memory for vertex/index buffers
		uint32_t m_nVertexBufferAvailableMemory;
		uint32_t m_nIndexBufferAvailableMemory;

		std::vector<uint8_t> m_pVertexMemory;
		std::vector<uint8_t> m_pIndexMemory;
	
		bool m_bLocked;
	};

}
