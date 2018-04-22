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

		m_pPreIB = NULL;
		memset(m_pPreVB, 0, sizeof(m_pPreVB));
		memset(m_preVBOffset, 0, sizeof(m_preVBOffset));
		m_pPrePipeline = NULL;
	}

	void VulkanRenderCommand::End()
	{
		VK_CHECK_RESULT(vkEndCommandBuffer(m_vkCmdBuffer));
	}

	void VulkanRenderCommand::SetIndexBuffer(IndexBuffer* pIB)
	{
		if (m_pPreIB == pIB)
			return;

		m_pPreIB = pIB;

		VulkanIndexBuffer* pIml = (VulkanIndexBuffer*)pIB;
		vkCmdBindIndexBuffer(m_vkCmdBuffer, pIml->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
	}

	void VulkanRenderCommand::SetVertexBuffer(int index, VertexBuffer* pVB, UINT nOffSet)
	{
		if (m_pPreVB[index] == pVB && m_preVBOffset[index] == nOffSet)
			return;

		m_pPreVB[index] = pVB;
		m_preVBOffset[index] = nOffSet;

		VulkanVertexBuffer* pIml = (VulkanVertexBuffer*)pVB;

		const VkDeviceSize offsets[1] = { nOffSet };

		vkCmdBindVertexBuffers(m_vkCmdBuffer, index, 1, &pIml->m_vertexBuffer.buffer, offsets);
	}

	void VulkanRenderCommand::SetTechnique(Technique* pTech)
	{
		VulkanTechnique* pVulkanTech = (VulkanTechnique*)(pTech);

		if (m_pPrePipeline != pVulkanTech->m_pPipline.get())
		{
			vkCmdBindPipeline(m_vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pVulkanTech->m_pPipline->m_pipeline);
		
			m_pPrePipeline = pVulkanTech->m_pPipline.get();
		}
		
		VkDescriptorSet descriptorSets[4];
		descriptorSets[0] = pVulkanTech->m_descriptorSets_uniform[0];
		descriptorSets[1] = pVulkanTech->m_descriptorSets_uniform[1];
		descriptorSets[2] = pVulkanTech->m_descriptorSets_sampler[0];
		descriptorSets[3] = pVulkanTech->m_descriptorSets_sampler[1];

		vkCmdBindDescriptorSets(m_vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pVulkanTech->m_pPipline->m_pipelineLayout, 0,
			4, descriptorSets, 0, NULL);
	}

	void VulkanRenderCommand::DrawIndex(UINT nIndexStart,UINT nIndexCount, UINT nInstanceCount, PRIMITIVE_TYPE ePrType)
	{
		vkCmdDrawIndexed(m_vkCmdBuffer, nIndexCount, nInstanceCount, nIndexStart, 0, 0);
	}

}


