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

	void VulkanRenderCommand::Create(uint32_t queueFamilyIndex)
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
		m_prePipeline = -1;
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
		vkCmdBindIndexBuffer(m_vkCmdBuffer, pIml->m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
	}

	void VulkanRenderCommand::SetVertexBuffer(int index, VertexBuffer* pVB, uint32_t nOffSet)
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
		VulkanShaderProgram* pVulkanShader = (VulkanShaderProgram*)(pTech->GetShaderProgram());

		if (m_prePipeline != pVulkanShader->m_graphicPip._Pipeline)
		{
			vkCmdBindPipeline(m_vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pVulkanShader->m_graphicPip._Pipeline);
		
			m_prePipeline = pVulkanShader->m_graphicPip._Pipeline;
		}
		
		vkCmdBindDescriptorSets(m_vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pVulkanShader->m_graphicPip._Layout, 0,
			1, &pVulkanTech->m_descriptorSet, 0, NULL);
	}

	void VulkanRenderCommand::DrawIndex(uint32_t nIndexStart,uint32_t nIndexCount, uint32_t nVertexStart, uint32_t nInstanceCount, PRIMITIVE_TYPE ePrType)
	{
		vkCmdDrawIndexed(m_vkCmdBuffer, nIndexCount, nInstanceCount, nIndexStart, nVertexStart, 0);
	}

	void VulkanRenderCommand::SetScissor(uint32_t firstScissor, uint32_t scissorCount, const Vector4* pScissors)
	{
		std::vector<VkRect2D> vecScissor(scissorCount);
		for (uint32_t i = 0; i < scissorCount; ++i)
		{
			VkRect2D scissor;
			scissor.offset.x = (uint32_t)pScissors[0].x;
			scissor.offset.y = (uint32_t)pScissors[0].y;
			scissor.extent.width = (uint32_t)pScissors[0].z;
			scissor.extent.height = (uint32_t)pScissors[0].w;
			vecScissor[i] = scissor;
		}
		vkCmdSetScissor(m_vkCmdBuffer, firstScissor, scissorCount, &vecScissor[0]);
	}


	VulkanComputeCommad::VulkanComputeCommad()
	{
		Create();
	}

	VulkanComputeCommad::~VulkanComputeCommad()
	{

	}

	void VulkanComputeCommad::Create()
	{
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		m_vkCmdBuffer = pRender->m_vkComputeCmdBuffer;
 
// 		// Separate command pool as queue family for compute may be different than graphics
// 		VkCommandPoolCreateInfo cmdPoolInfo = {};
// 		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
// 		cmdPoolInfo.queueFamilyIndex = vulkanDevice->queueFamilyIndices.compute;
// 		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
// 		VK_CHECK_RESULT(vkCreateCommandPool(vulkanDevice->logicalDevice, &cmdPoolInfo, nullptr, &m_vkCmdPool));
// 
// 		// Create a command buffer for compute operations
// 		VkCommandBufferAllocateInfo cmdBufAllocateInfo =
// 			vks::initializers::commandBufferAllocateInfo(
// 				m_vkCmdPool,
// 				VK_COMMAND_BUFFER_LEVEL_SECONDARY,
// 				1);
// 
// 		VK_CHECK_RESULT(vkAllocateCommandBuffers(vulkanDevice->logicalDevice, &cmdBufAllocateInfo, &m_vkCmdBuffer));

	}

	void VulkanComputeCommad::Begin()
	{

// 		// Inheritance info for the secondary command buffers
// 		VkCommandBufferInheritanceInfo inheritanceInfo = vks::initializers::commandBufferInheritanceInfo();
// 		inheritanceInfo.renderPass = m_pRenderPass->m_impl;
// 		// Secondary command buffer also use the currently active framebuffer
// 		inheritanceInfo.framebuffer = m_pRenderPass->m_frameBuffer;
// 
// 		VkCommandBufferBeginInfo commandBufferBeginInfo = vks::initializers::commandBufferBeginInfo();
// 		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
// 		commandBufferBeginInfo.pInheritanceInfo = &inheritanceInfo;
// 
// 		VK_CHECK_RESULT(vkBeginCommandBuffer(m_vkCmdBuffer, &cmdBufInfo));
	}

	void VulkanComputeCommad::End()
	{
//		VK_CHECK_RESULT(vkEndCommandBuffer(m_vkCmdBuffer));
	}

	void VulkanComputeCommad::SetTechnique(Technique* pTech)
	{
		VulkanTechnique* pVulkanTech = (VulkanTechnique*)(pTech);
		VulkanShaderProgram* pVulkanShader = (VulkanShaderProgram*)(pTech->GetShaderProgram());

		if (m_prePipeline != pVulkanShader->m_computePip._Pipeline)
		{
			vkCmdBindPipeline(m_vkCmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pVulkanShader->m_computePip._Pipeline);

			m_prePipeline = pVulkanShader->m_computePip._Pipeline;
		}

		vkCmdBindDescriptorSets(m_vkCmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pVulkanShader->m_computePip._Layout, 0,
			1, &pVulkanTech->m_descriptorSet, 0, NULL);
	}

	void VulkanComputeCommad::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		vkCmdDispatch(m_vkCmdBuffer, groupCountX, groupCountY, groupCountZ);
	}

}


