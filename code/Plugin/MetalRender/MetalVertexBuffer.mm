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
        /*
		ASSERT(mMetalVertexBuffer);
		if (mMetalVertexBuffer == NULL)
			return NULL;

		Metal_MAP D3DLock;

		if (LockFlag & LOCK_WRITE)
			D3DLock = Metal_MAP_WRITE;

		if (LockFlag & LOCK_READONLY)
			D3DLock = Metal_MAP_READ;

		if ((LockFlag & LOCK_DISCARD) && (m_Usage & USAGE_DYNAMIC))
			D3DLock = Metal_MAP_WRITE_DISCARD;

		if (LockFlag & LOCK_NOOVERWRITE)
			D3DLock = Metal_MAP_WRITE_NO_OVERWRITE;

		Metal_MAPPED_SUBRESOURCE mappedData;
		mappedData.pData = 0;

		GetMetalDxDeviveContext()->Map(mMetalVertexBuffer, 0, D3DLock, 0,&mappedData);
		ASSERT(mappedData.pData);
		if (mappedData.pData == NULL)
		{
			LogError("Failed to map vertex buffer");
		}

		return mappedData.pData;
      */
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
/*
		Metal_BUFFER_DESC bufferDesc;
		memset(&bufferDesc, 0, sizeof bufferDesc);
		bufferDesc.BindFlags = Metal_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = m_Usage & HBU_DYNAMIC ? Metal_CPU_ACCESS_WRITE : 0;
		if (m_Usage & HBU_DYNAMIC)
		{
			bufferDesc.Usage = Metal_USAGE_DYNAMIC;
		}
		else if (m_Usage == HBU_STATIC)
		{
			bufferDesc.Usage = Metal_USAGE_IMMUTABLE;
		}
		else
		{
			bufferDesc.Usage = Metal_USAGE_DEFAULT;
		}
		bufferDesc.ByteWidth = (UINT)(m_Size);

		Metal_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = m_pData;

		GetMetalDxDevive()->CreateBuffer(&bufferDesc, m_pData ? &InitData : NULL, &mMetalVertexBuffer);
		ASSERT(mMetalVertexBuffer);
		if (mMetalVertexBuffer == NULL)
		{
			LogError("CreateVertexBuffer failed");
			return;
		}
 */

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
