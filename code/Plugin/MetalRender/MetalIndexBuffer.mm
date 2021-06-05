#include "MetalIndexBuffer.h"
#include "MetalRenderDevice.h"

namespace ma
{


MetalIndexBuffer::MetalIndexBuffer()
{
    mMetalIndexBuffer = nil;
}

MetalIndexBuffer::~MetalIndexBuffer()
{
    [mMetalIndexBuffer release];
}


void* MetalIndexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
{
    void* pLock = [mMetalIndexBuffer contents];
    return (uint8_t*)pLock + iOffsetBytes;
}

void MetalIndexBuffer::UnlockImpl()
{
	
}

void MetalIndexBuffer::RT_StreamComplete()
{
    mMetalIndexBuffer = [GetMetalDevive() newBufferWithLength:m_Size options:MTLResourceOptionCPUCacheModeDefault];

    mMetalIndexBuffer.label = @"IndexBuffer";
    
}

void MetalIndexBuffer::RT_UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize)
{
    ASSERT(nSize <= m_Size);
    void* pLock = [mMetalIndexBuffer contents];
    memcpy(pLock, pData, std::min<uint32_t>(nSize,m_Size));
}

id<MTLBuffer> MetalIndexBuffer::GetMetalIndexBuffer()
{
    return mMetalIndexBuffer;
}

}
