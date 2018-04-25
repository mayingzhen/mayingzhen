#pragma once


namespace ma
{

	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass();

		~VulkanRenderPass();

	public:
		virtual void Begine();

		virtual void End();

		virtual void Create();

		virtual RenderCommand* GetThreadCommand(uint32_t nIndex, RenderListType eRLType);

		void InitRenderCommamd();

	private:
		void InitFrameBuffer();

	public:
		VkRenderPass m_impl = 0;

		VkFramebuffer m_frameBuffer = 0;

		std::vector<VkClearValue> m_vecClearValues;

		std::vector<VkCommandBuffer> m_vecActiceCmd;

		std::vector< RefPtr<VulkanRenderCommand> > m_arrRenderCommand;
	};

} 




