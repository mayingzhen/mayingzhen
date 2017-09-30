#include "VulkanRenderCommand.h"
#include "VulkanDebug.h"

namespace ma
{

	VulkanRenderCommand::VulkanRenderCommand()
	{

	}

	VulkanRenderCommand::~VulkanRenderCommand()
	{

	}

	void VulkanRenderCommand::Create(UINT queueFamilyIndex)
	{
		vks::VulkanDevice* vulkanDevice = GetVulkanDevice();

		VkCommandPoolCreateInfo cmdPoolInfo = vks::initializers::commandPoolCreateInfo();
		cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VK_CHECK_RESULT(vkCreateCommandPool(vulkanDevice->logicalDevice, &cmdPoolInfo, nullptr, &m_vkCmdPool));

		// Generate secondary command buffers for each thread
		VkCommandBufferAllocateInfo secondaryCmdBufAllocateInfo =
			vks::initializers::commandBufferAllocateInfo(
				m_vkCmdPool,
				VK_COMMAND_BUFFER_LEVEL_SECONDARY,
				1);
		VK_CHECK_RESULT(vkAllocateCommandBuffers(vulkanDevice->logicalDevice, &secondaryCmdBufAllocateInfo, &m_vkCmdBuffer));
	}

	void VulkanRenderCommand::Begin()
	{
		ASSERT(m_pRenderPass);
		if (m_pRenderPass == NULL)
			return;

		// Inheritance info for the secondary command buffers
		VkCommandBufferInheritanceInfo inheritanceInfo = vks::initializers::commandBufferInheritanceInfo();
		inheritanceInfo.renderPass = m_pRenderPass->m_impl;
		// Secondary command buffer also use the currently active framebuffer
		inheritanceInfo.framebuffer = m_pRenderPass->m_frameBuffer;

		VkCommandBufferBeginInfo commandBufferBeginInfo = vks::initializers::commandBufferBeginInfo();
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		commandBufferBeginInfo.pInheritanceInfo = &inheritanceInfo;

		VK_CHECK_RESULT(vkBeginCommandBuffer(m_vkCmdBuffer, &commandBufferBeginInfo));

		Rectangle rect = m_pRenderPass->m_viewPort;

		VkViewport viewport = vks::initializers::viewport((float)rect.width(), (float)rect.height(), 0.0f, 1.0f);
		viewport.x = rect.offsetX();
		viewport.y = rect.offsetY();
		vkCmdSetViewport(m_vkCmdBuffer, 0, 1, &viewport);

		VkRect2D scissor = vks::initializers::rect2D((int)rect.width(), (int)rect.height(), 0, 0);
		vkCmdSetScissor(m_vkCmdBuffer, 0, 1, &scissor);
	}

	void VulkanRenderCommand::End()
	{
		VK_CHECK_RESULT(vkEndCommandBuffer(m_vkCmdBuffer));

		m_pRenderPass = NULL;
	}

	void VulkanRenderCommand::SetIndexBuffer(IndexBuffer* pIB)
	{
		VulkanIndexBuffer* pIml = (VulkanIndexBuffer*)pIB;
		vkCmdBindIndexBuffer(m_vkCmdBuffer, pIml->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
	}

	void VulkanRenderCommand::SetVertexBuffer(int index, VertexBuffer* pVB)
	{
		VulkanVertexBuffer* pIml = (VulkanVertexBuffer*)pVB;

		const VkDeviceSize offsets[1] = { 0 };

		vkCmdBindVertexBuffers(m_vkCmdBuffer, 0, 1, &pIml->vertexBuffer.buffer, offsets);
	}

	void VulkanRenderCommand::DrawRenderable(const Renderable* pRenderable, Technique* pTech)
	{
		if (pRenderable == NULL)
			return;

		const RefPtr<SubMeshData>& pSubMeshData = pRenderable->m_pSubMeshData;

		UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : pRenderable->m_pIndexBuffer->GetNumber();
		UINT nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;

		vkCmdDrawIndexed(m_vkCmdBuffer, nIndexCount, 1, 0, nIndexStart, 0);
	}

}


