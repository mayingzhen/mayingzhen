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
    //MTLResourceOptions options;
    //{
    //    bool isWriteCombinedBuffer = (usage == RESOURCE_USAGE_STATIC_DRAW) || (usage == RESOURCE_USAGE_DYNAMIC_DRAW) || (usage == RESOURCE_USAGE_STREAM_DRAW);
    //    options = (isWriteCombinedBuffer ? MTLResourceCPUCacheModeWriteCombined : MTLResourceCPUCacheModeDefaultCache) | kDefaultBufferStorageMode << MTLResourceStorageModeShift;
   // }
    //if (m_Usage & HBU_DYNAMIC)
    //{
    //    options = MTLResourceCPUCacheModeWriteCombined;
    //}
    //else if (m_Usage == HBU_STATIC)
    //{
    //   options = MTLResourceCPUCacheModeDefaultCache;
    //}
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
    
    /*
	ASSERT(m_Size);

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof bufferDesc);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = m_Usage & HBU_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
	if (m_Usage & HBU_DYNAMIC)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	}
	else if (m_Usage == HBU_STATIC)
	{
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	}
	else
	{
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	}
	bufferDesc.ByteWidth = (UINT)(m_Size);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_pData;

	GetD3D11DxDevive()->CreateBuffer(&bufferDesc, m_pData ? &InitData : NULL, &mMetalIndexBuffer);
	ASSERT(mMetalIndexBuffer);
	if (mMetalIndexBuffer == NULL)
	{
		LogError("CreateVertexBuffer failed");
		return;
	}

	if (!m_bShadowData)
	{
		FreeData();
	}
     */
}

id<MTLBuffer> MetalIndexBuffer::GetMetalIndexBuffer()
{
    return mMetalIndexBuffer;
}

}
