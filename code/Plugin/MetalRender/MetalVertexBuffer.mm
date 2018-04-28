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
        
        if (m_pData == NULL)
        {
            mMetalVertexBuffer = [GetMetalDevive() newBufferWithLength:m_Size options:MTLResourceOptionCPUCacheModeDefault];
        }
        else
        {
            mMetalVertexBuffer = [GetMetalDevive() newBufferWithBytes:m_pData length:m_Size options:MTLResourceOptionCPUCacheModeDefault];
        }
        mMetalVertexBuffer.label = @"VertexBuffer";
        
        if (!m_bShadowData)
        {
            FreeData();
        }
	}

	id<MTLBuffer> MetalVertexBuffer::GetMetalVertexBuffer()
	{
		return mMetalVertexBuffer;
	}
}
