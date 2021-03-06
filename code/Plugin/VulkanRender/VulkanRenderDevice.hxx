#include "VulkanRenderDevice.h"
#include "VulkanTexture.h"
#include "VulkanConstantBuffer.h"
#include "VulkanRenderState.h"
#include "VulkanSamplerState.h"
#include "VulkanTechniqueh.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderCommand.h"

#include <array>
#include <thread>
#include "Engine\RenderSystem\RenderQueue.h"
#include "util_init.hpp"
#include "VulkanDebug.h"

namespace ma
{

	vks::VulkanDevice* GetVulkanDevice()
	{
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();
		ASSERT(pRender);
		return pRender->GetVulkanDevice();
	}

	VulkanRenderDevice::VulkanRenderDevice()
	{
	}

	VulkanRenderDevice::~VulkanRenderDevice()
	{
	}

	void VulkanRenderDevice::ClearAllStates()
	{
	}

	Texture* VulkanRenderDevice::CreateTexture()
	{
		return new VulkanTexture();
	}

	Texture* VulkanRenderDevice::CreateRenderTarget(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType)
	{
		return new VulkanTexture(nWidth,nHeight,nMipMap,format,bSRGB, TEXTURE_USAGE::USAGE_RENDERTARGET,eType);
	}

	Texture* VulkanRenderDevice::CreateDepthStencil(int nWidth,int nHeight,PixelFormat format)
	{
		return new VulkanTexture(nWidth,nHeight,1,format,false, TEXTURE_USAGE::USAGE_DEPTHSTENCIL,TEXTYPE_2D);
	}

	VertexDeclaration* VulkanRenderDevice::CreateVertexDeclaration()
	{
		return new VulkanVertexDeclaration();
	}

	VertexBuffer*	VulkanRenderDevice::CreateVertexBuffer()
	{
		return new VulkanVertexBuffer();
	}

	IndexBuffer*	VulkanRenderDevice::CreateIndexBuffer()
	{
		return new VulkanIndexBuffer();
	}

	ShaderProgram*	VulkanRenderDevice::CreateShaderProgram()
	{
		return new VulkanShaderProgram();
	}

	ConstantBuffer*	VulkanRenderDevice::CreateConstantBuffer(uint32_t nSize)
	{
		VulkanConstantBuffer* pVKConstBuffer = new VulkanConstantBuffer();
		pVKConstBuffer->SetSize(nSize);
		return pVKConstBuffer;
	}

	BlendState*	VulkanRenderDevice::CreateBlendState()
	{
		return new VulkanBlendState();
	}

	DepthStencilState*	VulkanRenderDevice::CreateDepthStencilState()
	{
		return new VulkanDepthStencilState();
	}

	RasterizerState* VulkanRenderDevice::CreateRasterizerState()
	{
		return new VulkanRasterizerState();
	}

	SamplerState* VulkanRenderDevice::CreateSamplerState()
	{
		return new VulkanSamplerStateObject();
	}

	Technique* VulkanRenderDevice::CreateTechnique()
	{
		return new VulkanTechnique();
	}

	RenderPass* VulkanRenderDevice::CreateRenderPass()
	{
		return new VulkanRenderPass();
	}

	RenderCommand* VulkanRenderDevice::CreateRenderCommand()
	{
		return new VulkanRenderCommand();
	}

	ComputeCommand* VulkanRenderDevice::CreateComputeCommand()
	{
		return new VulkanComputeCommand();
	}

	void VulkanRenderDevice::Shoutdown()
	{
	}

	void VulkanRenderDevice::Init(void* wndhandle)
	{
		layer_properties layer;
		memset(&layer.properties, 0, sizeof(layer.properties));
		init_global_extension_properties(layer);

		uint32_t InstanceLayerCount = 0;
		vkEnumerateInstanceLayerProperties(&InstanceLayerCount, NULL);

		std::vector<VkLayerProperties> vk_props(InstanceLayerCount);
		vkEnumerateInstanceLayerProperties(&InstanceLayerCount, vk_props.data());


		int32_t validationLayerCount = 1;
		const char* validationLayerNames[] = {
			"VK_LAYER_KHRONOS_validation"
		};

		bool enableValidation = false;
		for (auto& prop : vk_props)
		{
			if (strcmp(prop.layerName, validationLayerNames[0]) == 0)
			{
				enableValidation = true;
				break;
			}
		}


		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		//appInfo.pApplicationName = Game::GetInstance()->GetGameName();
		//appInfo.pEngineName = Game::GetInstance()->GetGameName();
		appInfo.apiVersion = VK_API_VERSION_1_2;

		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

		// Enable surface extensions depending on os
#if defined(_WIN32)
		instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		instanceExtensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
#elif defined(__ANDROID__)
		instanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_DIRECT2DISPLAY)
		instanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
		instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(__linux__)
		instanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

		instanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		instanceExtensions.push_back(VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME);
		//instanceExtensions.push_back(VK_KHR_MAINTENANCE1_EXTENSION_NAME);

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = NULL;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		if (instanceExtensions.size() > 0)
		{
 			if (enableValidation)
 			{
 				instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
 			}
			instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
			instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
		}
 		if (enableValidation)
 		{
 			instanceCreateInfo.enabledLayerCount = validationLayerCount;
			instanceCreateInfo.ppEnabledLayerNames = validationLayerNames;
 		}


 		VkResult err;
		err = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
		ASSERT(err == VK_SUCCESS);


#if defined(__ANDROID__)
        if(instance == NULL)
        {
            __android_log_print(ANDROID_LOG_INFO, "vulkanandroid", "vkCreateInstance Erroe...\n");
        }
	    vks::android::loadVulkanFunctions(instance);
#endif
		// Physical device
		uint32_t gpuCount = 0;
		// Get number of available physical devices
		VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));
		assert(gpuCount > 0);
		// Enumerate devices
		std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
		err = vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data());
		if (err)
		{
			//vks::tools::exitFatal("Could not enumerate physical devices : \n" + vks::tools::errorString(err), "Fatal error");
		}

		uint32_t selectedDevice = 0;
		for (uint32_t i = 0; i < gpuCount; ++i)
		{
			vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
			std::string strGpuName = deviceProperties.deviceName;
			if (strGpuName.find("GeForce") != std::string::npos)
			{
				selectedDevice = i;
				break;
			}
		}

		physicalDevice = physicalDevices[selectedDevice];

		// Store properties (including limits), features and memory properties of the phyiscal device (so that examples can check against them)
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

		LogInfo("GpuCount = %d, select %d, name %s", gpuCount, selectedDevice, deviceProperties.deviceName);

		// Queue family properties, used for setting up requested queues upon device creation
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		assert(queueFamilyCount > 0);
		queueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

		enabledExtensions.push_back(VK_KHR_MAINTENANCE1_EXTENSION_NAME);

		vulkanDevice = new vks::VulkanDevice(physicalDevice);
		VkResult res = vulkanDevice->createLogicalDevice(deviceFeatures, enabledExtensions);
		if (res != VK_SUCCESS) 
		{
			vks::tools::exitFatal("Could not create Vulkan device: \n" + vks::tools::errorString(res), "Fatal error");
		}

// 		if (enableValidation)
// 		{
// 			vks::debug::setupDebugging(instance, 0, nullptr);
// 		}

		vks::debugmarker::setup(vulkanDevice->logicalDevice);

		vkGetDeviceQueue(vulkanDevice->logicalDevice, vulkanDevice->queueFamilyIndices.graphics, 0, &m_queue);

		VkFenceCreateInfo fenceCreateInfo = vks::initializers::fenceCreateInfo(VK_FLAGS_NONE);
		vkCreateFence(vulkanDevice->logicalDevice, &fenceCreateInfo, NULL, &m_renderFence);

// 		// Create synchronization objects
 		//VkSemaphoreCreateInfo semaphoreCreateInfo = vks::initializers::semaphoreCreateInfo();
		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
 		// Create a semaphore used to synchronize image presentation
 		// Ensures that the image is displayed before we start submitting new commands to the queu
 		VK_CHECK_RESULT(vkCreateSemaphore(vulkanDevice->logicalDevice, &semaphoreCreateInfo, nullptr, &m_presentComplete));
 		// Create a semaphore used to synchronize command submission
 		// Ensures that the image is not presented until all commands have been sumbitted and executed
 		VK_CHECK_RESULT(vkCreateSemaphore(vulkanDevice->logicalDevice, &semaphoreCreateInfo, nullptr, &m_renderComplete));
// 		// Create a semaphore used to synchronize command submission
// 		// Ensures that the image is not presented until all commands for the text overlay have been sumbitted and executed
// 		// Will be inserted after the render complete semaphore if the text overlay is enabled
// 		VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphores.textOverlayComplete));
// 
		// Set up submit info structure
		// Semaphores will stay the same during application lifetime
		// Command buffer submission info is set by each example
		m_submitInfo = VkSubmitInfo{};
		m_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		m_submitInfo.pWaitDstStageMask = &m_submitPipelineStages;
		m_submitInfo.waitSemaphoreCount = 1;
		m_submitInfo.pWaitSemaphores = &m_presentComplete;
		m_submitInfo.signalSemaphoreCount = 1;
		m_submitInfo.pSignalSemaphores = &m_renderComplete;

		//setupSwapChain
		m_swapChain.connect(instance, physicalDevice, vulkanDevice->logicalDevice);
		m_swapChain.initSurface(wndhandle);
		m_swapChain.create(&m_width, &m_height, false);

		// createCommandBuffers
		VkCommandBufferAllocateInfo cmdBufAllocateInfo =
			vks::initializers::commandBufferAllocateInfo(
				vulkanDevice->commandPool,
				VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				1);

		VK_CHECK_RESULT(vkAllocateCommandBuffers(vulkanDevice->logicalDevice, &cmdBufAllocateInfo, &m_drawCmdBuffers));

		if (1)
		{
			// Create a compute capable device queue
			// The VulkanDevice::createLogicalDevice functions finds a compute capable queue and prefers queue families that only support compute
			// Depending on the implementation this may result in different queue family indices for graphics and computes,
			// requiring proper synchronization (see the memory barriers in buildComputeCommandBuffer)
			vkGetDeviceQueue(vulkanDevice->logicalDevice, vulkanDevice->queueFamilyIndices.compute, 0, &m_computeQueue);

			// Separate command pool as queue family for compute may be different than graphics
			VkCommandPoolCreateInfo cmdPoolInfo = {};
			cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			cmdPoolInfo.queueFamilyIndex = vulkanDevice->queueFamilyIndices.compute;
			cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			VK_CHECK_RESULT(vkCreateCommandPool(vulkanDevice->logicalDevice, &cmdPoolInfo, nullptr, &m_vkComputeCmdPool));

			// Create a command buffer for compute operations
			VkCommandBufferAllocateInfo cmdBufAllocateInfo =
				vks::initializers::commandBufferAllocateInfo(
					m_vkComputeCmdPool,
					VK_COMMAND_BUFFER_LEVEL_PRIMARY,
					1);

			VK_CHECK_RESULT(vkAllocateCommandBuffers(vulkanDevice->logicalDevice, &cmdBufAllocateInfo, &m_vkComputeCmdBuffer));

			// Fence for compute CB sync
			VkFenceCreateInfo fenceCreateInfo = vks::initializers::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
			VK_CHECK_RESULT(vkCreateFence(vulkanDevice->logicalDevice, &fenceCreateInfo, nullptr, &m_computeFence));
		}

		vks::debugmarker::setup(vulkanDevice->logicalDevice);

		m_depthFormat = VK_FORMAT_D24_UNORM_S8_UINT;

		SetupDepthStencil();
		
		SetupRenderPass();

		CreatePipelineCache();

		SetupFrameBuffer();
	}

	void VulkanRenderDevice::SetupDepthStencil()
	{
		// setupDepthStencil
		VkImageCreateInfo image = {};
		image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image.pNext = NULL;
		image.imageType = VK_IMAGE_TYPE_2D;
		image.format = m_depthFormat;
		image.extent = { m_width, m_height, 1 };
		image.mipLevels = 1;
		image.arrayLayers = 1;
		image.samples = VK_SAMPLE_COUNT_1_BIT;
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		image.flags = 0;

		VkMemoryAllocateInfo mem_alloc = {};
		mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		mem_alloc.pNext = NULL;
		mem_alloc.allocationSize = 0;
		mem_alloc.memoryTypeIndex = 0;

		VkImageViewCreateInfo depthStencilView = {};
		depthStencilView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		depthStencilView.pNext = NULL;
		depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		depthStencilView.format = m_depthFormat;
		depthStencilView.flags = 0;
		depthStencilView.subresourceRange = {};
		depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		depthStencilView.subresourceRange.baseMipLevel = 0;
		depthStencilView.subresourceRange.levelCount = 1;
		depthStencilView.subresourceRange.baseArrayLayer = 0;
		depthStencilView.subresourceRange.layerCount = 1;

		VkMemoryRequirements memReqs;

		VK_CHECK_RESULT(vkCreateImage(vulkanDevice->logicalDevice, &image, nullptr, &depthStencil.image));
		vkGetImageMemoryRequirements(vulkanDevice->logicalDevice, depthStencil.image, &memReqs);
		mem_alloc.allocationSize = memReqs.size;
		mem_alloc.memoryTypeIndex = vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(vulkanDevice->logicalDevice, &mem_alloc, nullptr, &depthStencil.mem));
		VK_CHECK_RESULT(vkBindImageMemory(vulkanDevice->logicalDevice, depthStencil.image, depthStencil.mem, 0));

		depthStencilView.image = depthStencil.image;
		VK_CHECK_RESULT(vkCreateImageView(vulkanDevice->logicalDevice, &depthStencilView, nullptr, &depthStencil.view));
	}

	void VulkanRenderDevice::SetupRenderPass()
	{
		std::array<VkAttachmentDescription, 2> attachments = {};
		// Color attachment
		attachments[0].format = m_swapChain.colorFormat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		// Depth attachment
		attachments[1].format = m_depthFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorReference = {};
		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthReference = {};
		depthReference.attachment = 1;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;
		subpassDescription.pDepthStencilAttachment = &depthReference;
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
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VkRenderPass renderPass;
		VK_CHECK_RESULT(vkCreateRenderPass(vulkanDevice->logicalDevice, &renderPassInfo, nullptr, &renderPass));

		m_pDefaultPass = new VulkanRenderPass();
		m_pDefaultPass->m_impl = renderPass;
		m_pDefaultPass->m_viewPort = Rectangle(0, 0, (float)m_width, (float)m_height);
		m_pDefaultPass->m_arrColor.resize(1);
		m_pDefaultPass->InitRenderCommamd();
	}

	void VulkanRenderDevice::CreatePipelineCache()
	{
		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VK_CHECK_RESULT(vkCreatePipelineCache(vulkanDevice->logicalDevice, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
	}

	void VulkanRenderDevice::SetupFrameBuffer()
	{
		VkImageView attachments[2];

		// Depth/Stencil attachment is the same for all frame buffers
		attachments[1] = depthStencil.view;

		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.pNext = NULL;
		frameBufferCreateInfo.renderPass = m_pDefaultPass->m_impl;
		frameBufferCreateInfo.attachmentCount = 2;
		frameBufferCreateInfo.pAttachments = attachments;
		frameBufferCreateInfo.width = m_width;
		frameBufferCreateInfo.height = m_height;
		frameBufferCreateInfo.layers = 1;

		// Create frame buffers for every swap chain image
		m_frameBuffers.resize(m_swapChain.imageCount);
		for (uint32_t i = 0; i < m_frameBuffers.size(); i++)
		{
			attachments[0] = m_swapChain.buffers[i].view;
			VK_CHECK_RESULT(vkCreateFramebuffer(vulkanDevice->logicalDevice, &frameBufferCreateInfo, nullptr, &m_frameBuffers[i]));
		}
	}


	bool VulkanRenderDevice::UpdateSwapChain(int width, int height)
	{

		return true;
	}
	
	bool VulkanRenderDevice::TestDeviceLost()
	{
		return false;
	}

	bool VulkanRenderDevice::Rest()
	{
		return true;
	}

	void VulkanRenderDevice::BegineCompute()
	{
		VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

		VK_CHECK_RESULT(vkBeginCommandBuffer(m_vkComputeCmdBuffer, &cmdBufInfo));
	}

	void VulkanRenderDevice::EndCompute()
	{
		VK_CHECK_RESULT(vkEndCommandBuffer(m_vkComputeCmdBuffer));

		VkSubmitInfo computeSubmitInfo = vks::initializers::submitInfo();
		computeSubmitInfo.commandBufferCount = 1;
		computeSubmitInfo.pCommandBuffers = &m_vkComputeCmdBuffer;

		VK_CHECK_RESULT(vkQueueSubmit(m_computeQueue, 1, &computeSubmitInfo, m_computeFence));

		// Submit compute commands
		vkWaitForFences(vulkanDevice->logicalDevice, 1, &m_computeFence, VK_TRUE, UINT64_MAX);
		vkResetFences(vulkanDevice->logicalDevice, 1, &m_computeFence);
	}

	void VulkanRenderDevice::BeginRender()
	{
		// Acquire the next image from the swap chain
		VkResult res = m_swapChain.acquireNextImage(m_presentComplete, &m_currentBuffer);
		assert(res == VK_SUCCESS);

		VkCommandBufferBeginInfo cmd_buf_info = {};
		cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmd_buf_info.pNext = NULL;
		cmd_buf_info.flags = 0;
		cmd_buf_info.pInheritanceInfo = NULL;

		res = vkBeginCommandBuffer(m_drawCmdBuffers, &cmd_buf_info);
		assert(res == VK_SUCCESS);

		m_pDefaultPass->m_frameBuffer = m_frameBuffers[m_currentBuffer];
	}

	void VulkanRenderDevice::EndRender()
	{
		VK_CHECK_RESULT(vkEndCommandBuffer(m_drawCmdBuffers));

		// Command buffer to be sumitted to the queue
 		m_submitInfo.commandBufferCount = 1;
 		m_submitInfo.pCommandBuffers = &m_drawCmdBuffers;
// 		m_submitInfo.waitSemaphoreCount = 1;
// 		m_submitInfo.pWaitSemaphores = &m_presentComplete;
// 		m_submitInfo.signalSemaphoreCount = 1;
// 		m_submitInfo.pSignalSemaphores = &m_renderComplete;

		//VkPipelineStageFlags stageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		//m_submitInfo.pWaitDstStageMask = &stageFlags;

		// Submit to queue
		VK_CHECK_RESULT(vkQueueSubmit(m_queue, 1, &m_submitInfo, m_renderFence));

		// Wait for fence to signal that all command buffers are ready
		VkResult fenceRes;
		do
		{
			fenceRes = vkWaitForFences(vulkanDevice->logicalDevice, 1, &m_renderFence, VK_TRUE, 100000000);
		} while (fenceRes == VK_TIMEOUT);
		VK_CHECK_RESULT(fenceRes);
		vkResetFences(vulkanDevice->logicalDevice, 1, &m_renderFence);

		VK_CHECK_RESULT(m_swapChain.queuePresent(m_queue, m_currentBuffer, m_renderComplete));

		VK_CHECK_RESULT(vkQueueWaitIdle(m_queue));
	}


	RenderPass* VulkanRenderDevice::GetBackBufferRenderPass()
	{
		return m_pDefaultPass.get();
	}

	Rectangle VulkanRenderDevice::GetViewport()
	{
		Rectangle rect;
		rect.right = (float)m_width;
		rect.bottom = (float)m_height;
		return rect;
	}

	//D3DXMatrixPerspectiveFovLH
	Matrix4 VulkanRenderDevice::MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf)
	{
		float yScale = 1.0f / Math::Tan(fovy * 0.5f);
		float xScale = yScale / Aspect;
		float zScale = zf / (zf - zn);

		out[0][0] = xScale; out[0][1] = 0.f;    out[0][2] = 0.f;    out[0][3] = 0.f;
		out[1][0] = 0.f;    out[1][1] = yScale; out[1][2] = 0.f;    out[1][3] = 0.f;
		out[2][0] = 0.f;    out[2][1] = 0.f;    out[2][2] = zScale; out[2][3] = -zn * zScale;
		out[3][0] = 0.f;    out[3][1] = 0.f;    out[3][2] = 1.0f;   out[3][3] = 0.f;

		//Vulkan clip space has inverted Y
// 		Matrix4 Clip = Matrix4::IDENTITY;
// 		Clip.setScale(Vector3(1,-1,1));
//  		out = Clip * out;

		return out;
	}

	//D3DXMatrixOrthoLH 
	Matrix4 VulkanRenderDevice::MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf)
	{
		out[0][0] = 2/width;
		out[1][1] = 2/height;
		out[2][2] = 1/(zf-zn);
		out[3][3] = 1.f;
		out[2][3] = -zn/(zf-zn);
		out[1][0] = out[2][0] = out[3][0] = 
			out[0][1] = out[0][2] = out[0][3] =
			out[1][2] = out[1][3] =
			out[2][1] = 
			out[3][1] = out[3][2] = 0;

// 		//Vulkan clip space has inverted Y
// 		Matrix4 Clip = Matrix4::IDENTITY;
// 		Clip.setScale(Vector3(1, -1, 1));
// 		out = Clip * out;

		return out;
	}

	Matrix4 VulkanRenderDevice::MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf)
	{
		out[0][0] = 2/(right-left);
		out[1][1] = 2/(top-bottom);
		out[2][2] = 1/(zn-zf);
		out[3][3] = 1.f;
		out[2][3] = zn/(zn-zf);
		out[1][0] = out[2][0] = out[3][0] = 
			out[0][1] = out[0][2] = 
			out[1][2] = 
			out[2][1] = 
			out[3][1] = out[3][2] = 0;
		out[0][3] = (left+right)/(left-right);
		out[1][3] = (top+bottom)/(bottom-top);
		return out;
	}

	void VulkanRenderDevice::BeginProfile(const char* pszLale)
	{ 
		float color[4] = { 0 };
		vks::debugmarker::beginRegion(m_drawCmdBuffers, pszLale, color);
	}

	void VulkanRenderDevice::EndProfile()
	{
		vks::debugmarker::endRegion(m_drawCmdBuffers);
	}

	void VulkanRenderDevice::NotifyResourceCreated(VulkanResource* pResource)
	{
	}

	void VulkanRenderDevice::NotifyResourceDestroyed(VulkanResource* pResource)
	{
	}

	bool VulkanRenderDevice::BuildDeviceCapabilities()
	{
		GetDeviceCapabilities()->log();
		return true;
	}
}



