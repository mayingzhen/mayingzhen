#include "VulkanIndexBuffer.h"

namespace ma
{


VulkanIndexBuffer::VulkanIndexBuffer()
{
	//mVulkanIndexBuffer = NULL;
}

VulkanIndexBuffer::~VulkanIndexBuffer()
{
   // SAFE_RELEASE(mVulkanIndexBuffer);
}


void* VulkanIndexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
{
	return NULL;
// 	Vulkan_MAP D3DLock;
// 
// 	if (LockFlag & LOCK_WRITE)
// 		D3DLock = Vulkan_MAP_WRITE;
// 
// 	if (LockFlag & LOCK_READONLY)
// 		D3DLock = Vulkan_MAP_READ;
// 
// 	if ((LockFlag & LOCK_DISCARD) && (m_Usage & USAGE_DYNAMIC))
// 		D3DLock = Vulkan_MAP_WRITE_DISCARD;
// 
// 	if (LockFlag & LOCK_NOOVERWRITE)
// 		D3DLock = Vulkan_MAP_WRITE_NO_OVERWRITE;
// 
// 
// 	Vulkan_MAPPED_SUBRESOURCE mappedData;
// 	mappedData.pData = 0;
// 
// 	GetVulkanDxDeviveContext()->Map(mVulkanIndexBuffer, 0, D3DLock, 0,&mappedData);
// 	ASSERT(mappedData.pData);
// 	if (mappedData.pData == NULL)
// 	{
// 		LogError("Failed to map vertex buffer");
// 	}
// 
// 	return mappedData.pData;
}

void VulkanIndexBuffer::UnlockImpl()
{
	//GetVulkanDxDeviveContext()->Unmap(mVulkanIndexBuffer, 0);
}

void VulkanIndexBuffer::RT_StreamComplete()
{
	vks::VulkanDevice* vulkanDevice = GetVulkanDevice();

	vks::Buffer indexStaging;

	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&indexStaging,
		static_cast<uint32_t>(m_Size),
		m_pData));
	// Target
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&indexBuffer,
		static_cast<uint32_t>((UINT)(m_Size))));

	VkCommandBuffer copyCmd = vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, false);

	// Copy
	VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();
	VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

	VkBufferCopy copyRegion = {};

	copyRegion.size = m_Size;
	vkCmdCopyBuffer(
		copyCmd,
		indexStaging.buffer,
		indexBuffer.buffer,
		1,
		&copyRegion);

	VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &copyCmd;

	VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

	VK_CHECK_RESULT(vkQueueSubmit(pRender->m_queue, 1, &submitInfo, VK_NULL_HANDLE));
	VK_CHECK_RESULT(vkQueueWaitIdle(pRender->m_queue));

	//todo: fence
	indexStaging.destroy();

	if (!m_bShadowData)
	{
		FreeData();
	}
}

// IVulkanBuffer* VulkanIndexBuffer::GetD3DIndexBuffer() 
// {
//     return mVulkanIndexBuffer;
// }

}
