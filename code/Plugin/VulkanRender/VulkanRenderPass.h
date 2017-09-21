#pragma once


namespace ma
{

	class VulkanRenderPass : public FrameBuffer
	{
	public:
		VulkanRenderPass();

		~VulkanRenderPass();

	public:

		void Begine(VkCommandBuffer commandBuffer);

		void End();

		virtual void Create();

	public:

		VkRenderPass m_impl = 0;

		VkCommandBuffer m_curCommand = 0;

		VkFramebuffer m_frameBuffer = 0;
		int m_width = 0;
		int m_height = 0;

		std::vector<VkClearValue> m_vecClearValues;
	};

} 




