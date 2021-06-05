#include "MetalVertexBuffer.h"
#include "MetalRenderDevice.h"

namespace ma
{

	MetalVertexBuffer::MetalVertexBuffer()
	{
		mMetalVertexBuffer = NULL;
	}

	MetalVertexBuffer::~MetalVertexBuffer()
	{
        [mMetalVertexBuffer release];
	}

	void* MetalVertexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
        void* pLock = [mMetalVertexBuffer contents];
        return (uint8_t*)pLock + iOffsetBytes;
	}

	void MetalVertexBuffer::UnlockImpl()
	{
        
	}

	void MetalVertexBuffer::RT_StreamComplete()
	{
		ASSERT(mMetalVertexBuffer == NULL);

		ASSERT(m_Size);
        
        mMetalVertexBuffer = [GetMetalDevive() newBufferWithLength:m_Size options:MTLResourceOptionCPUCacheModeDefault];
       
        mMetalVertexBuffer.label = @"VertexBuffer";
	}

    void MetalVertexBuffer::RT_UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize)
    {
        ASSERT(nSize <= m_Size);
        void* pLock = [mMetalVertexBuffer contents];
        memcpy(pLock, pData, std::min<uint32_t>(nSize,m_Size));
    }

	id<MTLBuffer> MetalVertexBuffer::GetMetalVertexBuffer()
	{
		return mMetalVertexBuffer;
	}
}
