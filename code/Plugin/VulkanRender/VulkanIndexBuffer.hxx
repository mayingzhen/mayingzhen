#include "VulkanIndexBuffer.h"

namespace ma
{


VulkanIndexBuffer::VulkanIndexBuffer()
{
}

VulkanIndexBuffer::~VulkanIndexBuffer()
{
	indexBuffer.destroy();
}


void* VulkanIndexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
{
	return NULL;
}

void VulkanIndexBuffer::UnlockImpl()
{

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
		static_cast<uint32_t>((uint32_t)(m_Size))));

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
