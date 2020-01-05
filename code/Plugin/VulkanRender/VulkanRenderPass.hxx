#include "VulkanRenderPass.h"
#include "VulkanTexture.h"
#include "VulkanRenderCommand.h"

namespace ma
{

	VulkanRenderPass::VulkanRenderPass()
	{
		m_vecClearValues.resize(2);
		m_vecClearValues[0].color = { { 0.025f, 0.025f, 0.025f, 1.0f } };
		m_vecClearValues[1].depthStencil = { 1.0f, 0 };

		m_nMaxStageCount = 7;

		uint32_t numThreads = std::thread::hardware_concurrency();
		m_arrRenderCommand.resize(numThreads * m_nMaxStageCount);
		for (uint32_t i = 0; i < m_arrRenderCommand.size(); ++i)
		{
			m_arrRenderCommand[i] = new VulkanRenderCommand();
			m_arrRenderCommand[i]->m_pRenderPass = this;
		}
	}

	VulkanRenderPass::~VulkanRenderPass()
	{

	}

	RenderCommand* VulkanRenderPass::GetThreadCommand(uint32_t nIndex, int stage)
	{
		uint32_t numThreads = std::thread::hardware_concurrency();

		uint32_t nAt = stage * numThreads + nIndex;
		ASSERT(nAt < m_arrRenderCommand.size());
		if (nAt >= m_arrRenderCommand.size())
			return NULL;

		VulkanRenderCommand* pRenderCmd = m_arrRenderCommand[nAt].get();
		ASSERT(pRenderCmd->m_pRenderPass);
		m_vecActiceCmd.push_back(pRenderCmd->m_vkCmdBuffer);
		return pRenderCmd;
	}

	void VulkanRenderPass::Begine(const Rectangle& viewPort)
	{
		VulkanRenderDevice* pRenderDevice = (VulkanRenderDevice*)GetRenderDevice();
		VkCommandBuffer vkMainCmd = pRenderDevice->m_drawCmdBuffers;

		Create();

		Rectangle area = viewPort.IsEmpty() ? viewPort : m_viewPort;

		VkRenderPassBeginInfo rp_begin = {};
		rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		rp_begin.pNext = NULL;
		rp_begin.renderPass = m_impl;
		rp_begin.framebuffer = m_frameBuffer;
		rp_begin.renderArea.offset.x = (int)area.offsetX();
		rp_begin.renderArea.offset.y = (int)area.offsetY();
		rp_begin.renderArea.extent.width = (uint32_t)area.width();
		rp_begin.renderArea.extent.height = (uint32_t)area.height();
		rp_begin.clearValueCount = m_vecClearValues.size();
		rp_begin.pClearValues = m_vecClearValues.data();

		vkCmdBeginRenderPass(vkMainCmd, &rp_begin, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS/*VK_SUBPASS_CONTENTS_INLINE*/);
	}

	void VulkanRenderPass::End()
	{
		VulkanRenderDevice* pRenderDevice = (VulkanRenderDevice*)GetRenderDevice();
		VkCommandBuffer vkMainCmd = pRenderDevice->m_drawCmdBuffers;

		if (!m_vecActiceCmd.empty())
		{
			vkCmdExecuteCommands(vkMainCmd, m_vecActiceCmd.size(), m_vecActiceCmd.data());
		}

		vkCmdEndRenderPass(vkMainCmd);

		m_vecActiceCmd.clear();
	}

	void VulkanRenderPass::Create()
	{
		vks::VulkanDevice* device = GetVulkanDevice();
		
		if (m_impl != 0)
			return;

		m_vecClearValues.clear();

		std::vector<VkAttachmentDescription> vecAttachments;

		std::vector<VkAttachmentReference> vecColorReference;
		for (uint32_t i = 0; i < m_arrColor.size(); ++i)
		{
			VulkanTexture* pTexture = (VulkanTexture*)m_arrColor[i].m_pTexture.get();
			if (pTexture == NULL)
				continue;

			// Color attachment
			VkAttachmentDescription color = {};
			color.format = pTexture->m_vkformat;
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

			ColourValue vcolor = m_arrColor[i].m_cClearColor;

			VkClearValue clearValues;
			clearValues.color = { { vcolor.r, vcolor.g, vcolor.b, vcolor.a } };
			m_vecClearValues.push_back(clearValues);
		}

		if (m_pDepthStencil)
		{
			VulkanTexture* pTexture = (VulkanTexture*)m_pDepthStencil.get();

			// Depth attachment
			VkAttachmentDescription ds = {};
			ds.format = pTexture->m_vkformat;
			ds.samples = VK_SAMPLE_COUNT_1_BIT;
			ds.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			ds.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			ds.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			ds.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			ds.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			ds.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // Attachment will be transitioned to shader read at render pass end

			vecAttachments.push_back(ds);
			
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

		InitFrameBuffer();

		InitRenderCommamd();
	}

	void VulkanRenderPass::InitFrameBuffer()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		std::vector<VkImageView> vecImagView;

		std::vector<VkAttachmentReference> vecColorReference;
		for (uint32_t i = 0; i < m_arrColor.size(); ++i)
		{
			Texture* pTexture = m_arrColor[i].m_pTexture.get();
			if (pTexture == NULL)
				continue;

			VulkanTexture* pVkTexture = (VulkanTexture*)pTexture;
			vecImagView.push_back(pVkTexture->GetRenderTargetView(m_arrColor[i].m_nMip,m_arrColor[i].m_nFace));
		}

		if (m_pDepthStencil)
		{
			VulkanTexture* pVkTexture = (VulkanTexture*)m_pDepthStencil.get();
			vecImagView.push_back(pVkTexture->GetRenderTargetView(0,0));
		}

		Texture* pRT = m_arrColor.empty() ? m_pDepthStencil.get() : m_arrColor[0].m_pTexture.get();
		ASSERT(pRT);

		int nMip = m_arrColor.empty() ? 0 : m_arrColor[0].m_nMip;

		// Create frame buffer
		VkFramebufferCreateInfo fbufCreateInfo = vks::initializers::framebufferCreateInfo();
		fbufCreateInfo.renderPass = m_impl;
		fbufCreateInfo.attachmentCount = vecImagView.size();
		fbufCreateInfo.pAttachments = vecImagView.data();
		fbufCreateInfo.width = pRT->GetWidth() >> nMip;
		fbufCreateInfo.height = pRT->GetHeight() >> nMip;
		fbufCreateInfo.layers = 1;

		VK_CHECK_RESULT(vkCreateFramebuffer(device->logicalDevice, &fbufCreateInfo, nullptr, &m_frameBuffer));

		if (m_viewPort.width() == 0 || m_viewPort.height() == 0)
		{
			m_viewPort = Rectangle(0, 0, (float)fbufCreateInfo.width, (float)fbufCreateInfo.height);
		}
	}

	void VulkanRenderPass::InitRenderCommamd()
	{
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		for (uint32_t i = 0; i < m_arrRenderCommand.size(); ++i)
		{
			VulkanRenderCommand* pRenderCommad = m_arrRenderCommand[i].get();
			pRenderCommad->Create(pRender->m_swapChain.queueNodeIndex);
		}
	}
}

