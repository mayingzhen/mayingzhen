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
	
		bool m_bLocked;
	};

	class TransientParallHardWareBuffer : public Referenced
	{
	public:
		enum { nNumBuffer = 4 };

		TransientParallHardWareBuffer(uint32_t nVertexStride, uint32_t nNumVertice, uint32_t numIndexes)
		{
			for (uint32_t i = 0; i < nNumBuffer; ++i)
			{
				m_pBuffer[i] = new ParallHardWareBuffer(nVertexStride, nNumVertice, numIndexes);
			}
		}

		void BeginFrame()
		{
			m_pBuffer[m_nFillIndex]->LockVideoMemory();
		}

		void EndFrame()
		{
			m_pBuffer[m_nFillIndex]->UnLockVideoMemory();

			m_nProcessIndex = m_nFillIndex;
			m_nFillIndex = (m_nProcessIndex + 1) % nNumBuffer;
		}

		ParallHardWareBuffer* GetParallHardWareBuffer()
		{
			return m_pBuffer[m_nFillIndex].get();
		}
		

	private:
		RefPtr<ParallHardWareBuffer> m_pBuffer[nNumBuffer];

		uint32_t m_nFillIndex = 0;
		uint32_t m_nProcessIndex = 0;
	};

}
