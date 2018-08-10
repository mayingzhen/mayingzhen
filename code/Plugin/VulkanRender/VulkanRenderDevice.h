#pragma once

#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "Engine\RenderSystem\RenderQueue.h"

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class VulkanConstantBuffer;
	class VulkanResource;
	class VulkanShaderProgram;
	class VulkanVertexDeclaration;
	class VulkanRenderPass;
	class VulkanRenderCommand;

	class VulkanRenderDevice : public IRenderDevice
	{
	public:
		VulkanRenderDevice();

		~VulkanRenderDevice();

		virtual RenderDeviceType	GetRenderDeviceType() {return RenderDevice_VULKAN;}

		virtual Texture*			CreateTexture();
		virtual Texture*			CreateRenderTarget(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType);
		virtual Texture*			CreateDepthStencil(int nWidth,int nHeight,PixelFormat format);
		virtual VertexDeclaration*	CreateVertexDeclaration();
		virtual VertexBuffer*		CreateVertexBuffer();
		virtual IndexBuffer*		CreateIndexBuffer();
		virtual ShaderProgram*		CreateShaderProgram();
		virtual ConstantBuffer*		CreateConstantBuffer(uint32_t nSize);
		virtual BlendState*			CreateBlendState();
		virtual DepthStencilState*	CreateDepthStencilState();
		virtual RasterizerState*	CreateRasterizerState();
		virtual SamplerState*		CreateSamplerState();
		virtual Technique*			CreateTechnique();
		virtual RenderPass*			CreateRenderPass();
		virtual RenderCommand*		CreateRenderCommand();
		virtual ComputeCommad*		CreateComputeCommand();

		virtual Rectangle			GetViewport();
		virtual RenderPass*			GetDefaultRenderPass();

		virtual	void				Init(void* wndhandle);
		virtual void				Shoutdown();
		virtual bool				TestDeviceLost();
		virtual	bool				Rest();
		virtual void				BeginRender();
		virtual void				EndRender();

		virtual void				BegineCompute();
		virtual void				EndCompute();
		
		virtual	Matrix4				MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf);
		virtual	Matrix4				MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf);
		virtual Matrix4				MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf);

		virtual	void				BeginProfile(const char* pszLale);
		virtual	void				EndProfile();

		// Help fun
		virtual	bool				CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage);

		vks::VulkanDevice*			GetVulkanDevice() { return vulkanDevice; }

		void						NotifyResourceCreated(VulkanResource* pRes);

		void						NotifyResourceDestroyed(VulkanResource* pRes);

		void						ClearAllStates();

	private:
		bool						BuildDeviceCapabilities();

		bool						UpdateSwapChain(int width, int height);

		void						SetupDepthStencil();

		void						SetupRenderPass();

		void						CreatePipelineCache();

		void						SetupFrameBuffer();
	
	public:
		// Vulkan instance, stores all per-application states
		VkInstance instance;
		// Physical device (GPU) that Vulkan will ise
		VkPhysicalDevice physicalDevice;
		// Stores physical device properties (for e.g. checking device limits)
		VkPhysicalDeviceProperties deviceProperties;
		// Stores the features available on the selected physical device (for e.g. checking if a feature is available)
		VkPhysicalDeviceFeatures deviceFeatures;
		// Stores all available memory (type) properties for the physical device
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

		std::vector<VkQueueFamilyProperties> queueFamilyProperties;
		std::vector<std::string> supportedExtensions;

		uint32_t m_width = 1280;
		uint32_t m_height = 720;

		/** @brief Default command pool for the graphics queue family index */
		//VkCommandPool commandPool = VK_NULL_HANDLE;
	
		VkQueue m_queue;

		VulkanSwapChain m_swapChain;

		// Swap chain image presentation
		VkSemaphore m_presentComplete;
		// Command buffer submission and execution
		VkSemaphore m_renderComplete;

		VkSubmitInfo m_submitInfo;

		VkPipelineStageFlags m_submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkFence m_renderFence = {};

		VkCommandBuffer m_drawCmdBuffers;

		uint32_t m_currentBuffer = 0;

		RefPtr<VulkanRenderPass> m_pDefaultPass;

		VulkanRenderPass* m_pCurRenderPass = NULL;

		VkPipelineCache m_pipelineCache;

		std::vector<VkFramebuffer> m_frameBuffers;

		VkCommandBuffer m_vkComputeCmdBuffer;
		VkCommandPool m_vkComputeCmdPool;
		VkFence m_computeFence;
		VkQueue m_computeQueue;

		struct
		{
			VkImage image;
			VkDeviceMemory mem;
			VkImageView view;
		} depthStencil;

		VkFormat m_depthFormat;

		vks::VulkanDevice* vulkanDevice;

		//VkPhysicalDeviceFeatures enabledFeatures{};
		std::vector<const char*> enabledExtensions;
	};

	vks::VulkanDevice* GetVulkanDevice();
}


