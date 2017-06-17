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
    //SAFE_RELEASE(mMetalIndexBuffer);
}


void* MetalIndexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
{
    return NULL;
}

void MetalIndexBuffer::UnlockImpl()
{
	//GetD3D11DxDeviveContext()->Unmap(mMetalIndexBuffer, 0);
}

void MetalIndexBuffer::RT_StreamComplete()
{
    if (m_pData == NULL)
    {
        mMetalIndexBuffer = [GetMetalDevive() newBufferWithLength:m_Size options:MTLResourceOptionCPUCacheModeDefault];
    }
    else
    {
        mMetalIndexBuffer = [GetMetalDevive() newBufferWithBytes:m_pData length:m_Size options:MTLResourceOptionCPUCacheModeDefault];
    }
    mMetalIndexBuffer.label = @"IndexBuffer";
    
    if (!m_bShadowData)
    {
        FreeData();
    }
}

id<MTLBuffer> MetalIndexBuffer::GetMetalIndexBuffer()
{
    return mMetalIndexBuffer;
}

}
