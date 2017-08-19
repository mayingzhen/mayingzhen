#include "VulkanVertexBuffer.h"

namespace ma
{

	VulkanVertexBuffer::VulkanVertexBuffer()
	{
		//mVulkanVertexBuffer = NULL;
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		//SAFE_RELEASE(mVulkanVertexBuffer);
	}

	void* VulkanVertexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
// 		ASSERT(mVulkanVertexBuffer);
// 		if (mVulkanVertexBuffer == NULL)
// 			return NULL;
// 
// 		Vulkan_MAP D3DLock;
// 
// 		if (LockFlag & LOCK_WRITE)
// 			D3DLock = Vulkan_MAP_WRITE;
// 
// 		if (LockFlag & LOCK_READONLY)
// 			D3DLock = Vulkan_MAP_READ;
// 
// 		if ((LockFlag & LOCK_DISCARD) && (m_Usage & USAGE_DYNAMIC))
// 			D3DLock = Vulkan_MAP_WRITE_DISCARD;
// 
// 		if (LockFlag & LOCK_NOOVERWRITE)
// 			D3DLock = Vulkan_MAP_WRITE_NO_OVERWRITE;
// 
// 		Vulkan_MAPPED_SUBRESOURCE mappedData;
// 		mappedData.pData = 0;
// 
// 		GetVulkanDxDeviveContext()->Map(mVulkanVertexBuffer, 0, D3DLock, 0,&mappedData);
// 		ASSERT(mappedData.pData);
// 		if (mappedData.pData == NULL)
// 		{
// 			LogError("Failed to map vertex buffer");
// 		}
// 
// 		return mappedData.pData;

		return NULL;
	}

	void VulkanVertexBuffer::UnlockImpl()
	{
// 		ASSERT(mVulkanVertexBuffer);
// 		if (mVulkanVertexBuffer == NULL)
// 			return;
// 
// 		GetVulkanDxDeviveContext()->Unmap(mVulkanVertexBuffer, 0);
	}

	void VulkanVertexBuffer::RT_StreamComplete()
	{
// 		ASSERT(mVulkanVertexBuffer == NULL);
// 
// 		ASSERT(m_Size);
// 
// 		Vulkan_BUFFER_DESC bufferDesc;
// 		memset(&bufferDesc, 0, sizeof bufferDesc);
// 		bufferDesc.BindFlags = Vulkan_BIND_VERTEX_BUFFER;
// 		bufferDesc.CPUAccessFlags = m_Usage & HBU_DYNAMIC ? Vulkan_CPU_ACCESS_WRITE : 0;
// 		if (m_Usage & HBU_DYNAMIC)
// 		{
// 			bufferDesc.Usage = Vulkan_USAGE_DYNAMIC;
// 		}
// 		else if (m_Usage == HBU_STATIC)
// 		{
// 			bufferDesc.Usage = Vulkan_USAGE_IMMUTABLE;
// 		}
// 		else
// 		{
// 			bufferDesc.Usage = Vulkan_USAGE_DEFAULT;
// 		}
// 		bufferDesc.ByteWidth = (UINT)(m_Size);
// 
// 		Vulkan_SUBRESOURCE_DATA InitData;
// 		InitData.pSysMem = m_pData;
// 
// 		GetVulkanDxDevive()->CreateBuffer(&bufferDesc, m_pData ? &InitData : NULL, &mVulkanVertexBuffer);
// 		ASSERT(mVulkanVertexBuffer);
// 		if (mVulkanVertexBuffer == NULL)
// 		{
// 			LogError("CreateVertexBuffer failed");
// 			return;
// 		}
// 
// 		if (!m_bShadowData)
// 		{
// 			FreeData();
// 		}
	}

// 	IVulkanBuffer * VulkanVertexBuffer::GetD3DVertexBuffer()
// 	{
// 		return mVulkanVertexBuffer;
// 	}
}
