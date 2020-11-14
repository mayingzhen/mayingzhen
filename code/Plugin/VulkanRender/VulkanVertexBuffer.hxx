#include "VulkanVertexBuffer.h"

namespace ma
{

	VulkanVertexBuffer::VulkanVertexBuffer()
	{
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		m_vertexBuffer.destroy();
	}

	void* VulkanVertexBuffer::LockImpl(int iOffsetBytes, int iLockSize, LOCK LockFlag)
	{
		if (m_vertexBuffer.device == VK_NULL_HANDLE)
			return NULL;

		VK_CHECK_RESULT(m_vertexBuffer.map(iLockSize, iOffsetBytes));
		
		return m_vertexBuffer.mapped;
	}

	void VulkanVertexBuffer::UnlockImpl()
	{
		m_vertexBuffer.unmap();
	}

	void VulkanVertexBuffer::RT_UpdateData(uint32_t nOffset, uint8_t* pData, uint32_t nSize)
	{
		if (m_vertexBuffer.device == VK_NULL_HANDLE)
			return;

		VK_CHECK_RESULT(m_vertexBuffer.map(nSize, nOffset));

		memcpy(m_vertexBuffer.mapped, pData, nSize);

		m_vertexBuffer.unmap();
	}

	void VulkanVertexBuffer::RT_StreamComplete()
	{
		vks::VulkanDevice* vulkanDevice = GetVulkanDevice();

		if (GetUsage() == HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE ||
			GetUsage() == HBU_DYNAMIC)
		{
			VK_CHECK_RESULT(vulkanDevice->createBuffer(
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				&m_vertexBuffer,
				static_cast<uint32_t>(m_Size)));
		}
		else if (GetUsage() == HBU_STORGE_BUFFER || GetUsage() == HBU_STORGE_VECTOR_BUFFER)
		{
			vks::Buffer vertexStaging;

			VK_CHECK_RESULT(vulkanDevice->createBuffer(
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				&vertexStaging,
				static_cast<uint32_t>(m_Size),
				m_pData));
			// Target
			VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			if (GetUsage() == HBU_STORGE_VECTOR_BUFFER)
				usageFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			VK_CHECK_RESULT(vulkanDevice->createBuffer(
				usageFlags,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				&m_vertexBuffer,
				static_cast<uint32_t>((uint32_t)(m_Size))));

			VkCommandBuffer copyCmd = vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, false);

			// Copy
			VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();
			VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

			VkBufferCopy copyRegion = {};

			copyRegion.size = m_Size;
			vkCmdCopyBuffer(
				copyCmd,
				vertexStaging.buffer,
				m_vertexBuffer.buffer,
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
			vertexStaging.destroy();

			if (!m_bShadowData)
			{
				FreeData();
			}
		}
		else
		{
			vks::Buffer vertexStaging;

			VK_CHECK_RESULT(vulkanDevice->createBuffer(
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				&vertexStaging,
				static_cast<uint32_t>(m_Size),
				m_pData));
			// Target
			VK_CHECK_RESULT(vulkanDevice->createBuffer(
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				&m_vertexBuffer,
				static_cast<uint32_t>((uint32_t)(m_Size))));

			VkCommandBuffer copyCmd = vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, false);

			// Copy
			VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();
			VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

			VkBufferCopy copyRegion = {};

			copyRegion.size = m_Size;
			vkCmdCopyBuffer(
				copyCmd,
				vertexStaging.buffer,
				m_vertexBuffer.buffer,
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
			vertexStaging.destroy();

			if (!m_bShadowData)
			{
				FreeData();
			}
		}
	}
}

