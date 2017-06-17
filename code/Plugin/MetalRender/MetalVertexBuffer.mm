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
		//SAFE_RELEASE(mMetalVertexBuffer);
	}

	void* MetalVertexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
        return NULL;
	}

	void MetalVertexBuffer::UnlockImpl()
	{
		ASSERT(mMetalVertexBuffer);
		if (mMetalVertexBuffer == NULL)
			return;

		//GetMetalDxDeviveContext()->Unmap(mMetalVertexBuffer, 0);
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
