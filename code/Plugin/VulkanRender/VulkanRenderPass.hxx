#include "VulkanRenderPass.h"
#include "VulkanTexture.h"

namespace ma
{

	VulkanRenderPass::VulkanRenderPass()
	{
		m_vecClearValues.resize(2);
		m_vecClearValues[0].color = { { 0.025f, 0.025f, 0.025f, 1.0f } };
		m_vecClearValues[1].depthStencil = { 1.0f, 0 };
	}

	VulkanRenderPass::~VulkanRenderPass()
	{

	}

	void VulkanRenderPass::Begine(VkCommandBuffer commandBuffer)
	{
		m_curCommand = commandBuffer;

		Create();

		VkRenderPassBeginInfo rp_begin = {};
		rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		rp_begin.pNext = NULL;
		rp_begin.renderPass = m_impl;
		rp_begin.framebuffer = m_frameBuffer;
		rp_begin.renderArea.offset.x = 0;
		rp_begin.renderArea.offset.y = 0;
		rp_begin.renderArea.extent.width = m_width;
		rp_begin.renderArea.extent.height = m_height;
		rp_begin.clearValueCount = m_vecClearValues.size();
		rp_begin.pClearValues = m_vecClearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanRenderPass::End()
	{
		vkCmdEndRenderPass(m_curCommand);
	}

	void VulkanRenderPass::Create()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		if (m_impl != 0)
			return;

		m_vecClearValues.clear();

		std::vector<VkAttachmentDescription> vecAttachments;
		std::vector<VkImageView> vecImagView;

		std::vector<VkAttachmentReference> vecColorReference;
		for (UINT i = 0; i < MAX_RENDERTARGETS; ++i)
		{
			if (m_arrColor[i] == NULL)
				continue;

			// Color attachment
			VkAttachmentDescription color = {};
			color.format = VulkanMapping::_getPF( m_arrColor[i]->GetFormat() );
			color.samples = VK_SAMPLE_COUNT_1_BIT;
			color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			color.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			color.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			vecAttachments.push_back(color);

			VkAttachmentReference colorReference = {};
			colorReference.attachment = i;
			colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			vecColorReference.push_back(colorReference);

			VulkanTexture* pVkTexture = (VulkanTexture*)m_arrColor[i].get();
			vecImagView.push_back(pVkTexture->m_view);

			VkClearValue clearValues;
			clearValues.color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
			m_vecClearValues.push_back(clearValues);
		}

		if (m_pDepthStencil)
		{
			// Depth attachment
			VkAttachmentDescription ds = {};
			ds.format = VulkanMapping::_getPF( m_pDepthStencil->GetFormat() );
			ds.samples = VK_SAMPLE_COUNT_1_BIT;
			ds.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			ds.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			ds.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			ds.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			ds.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			ds.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL; // Attachment will be transitioned to shader read at render pass end

			vecAttachments.push_back(ds);
			
			VulkanTexture* pVkTexture = (VulkanTexture*)m_pDepthStencil.get();
			vecImagView.push_back(pVkTexture->m_view);

			VkClearValue clearValues;
			clearValues.depthStencil = { 1.0f, 0 };

			m_vecClearValues.push_back(clearValues);
		}

		VkAttachmentReference depthReference = {};
		depthReference.attachment = vecColorReference.size();
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = vecColorReference.size();
		subpassDescription.pColorAttachments = vecColorReference.data();
		subpassDescription.pDepthStencilAttachment = m_pDepthStencil ? &depthReference : NULL;
		subpassDescription.inputAttachmentCount = 0;
		subpassDescription.pInputAttachments = nullptr;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = nullptr;
		subpassDescription.pResolveAttachments = nullptr;

		// Subpass dependencies for layout transitions
		std::array<VkSubpassDependency, 2> dependencies;

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = vecAttachments.size();
		renderPassInfo.pAttachments = vecAttachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VK_CHECK_RESULT(vkCreateRenderPass(device->logicalDevice, &renderPassInfo, nullptr, &m_impl));

		Texture* pRT = m_arrColor[0] ? m_arrColor[0].get() : m_pDepthStencil.get();
		ASSERT(pRT);
		m_width = pRT->GetWidth();
		m_height = pRT->GetHeight();

		// Create frame buffer
		VkFramebufferCreateInfo fbufCreateInfo = vks::initializers::framebufferCreateInfo();
		fbufCreateInfo.renderPass = m_impl;
		fbufCreateInfo.attachmentCount = vecImagView.size();
		fbufCreateInfo.pAttachments = vecImagView.data();
		fbufCreateInfo.width = m_width;
		fbufCreateInfo.height = m_height;
		fbufCreateInfo.layers = 1;

		VK_CHECK_RESULT(vkCreateFramebuffer(device->logicalDevice, &fbufCreateInfo, nullptr, &m_frameBuffer));

	}

}

