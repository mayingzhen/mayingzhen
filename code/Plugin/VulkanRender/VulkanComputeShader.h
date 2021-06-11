#pragma once


namespace ma
{

	class VulkanComputeShader : public ComputeShader
	{
	public:

		VulkanComputeShader();

		~VulkanComputeShader();

	private:
		virtual void RT_StreamComplete();

		void Destory();

		void CreateDescriptorPool();

		void CreateComputePipeline(const VkPipelineShaderStageCreateInfo& stage);

	public:
		VkDescriptorPool m_desc_pool = VK_NULL_HANDLE;

		VkDescriptorSetLayout m_desc_layout = VK_NULL_HANDLE;

		VkPipelineCache m_cache = VK_NULL_HANDLE;

		VkPipeline m_pipeline = VK_NULL_HANDLE;

		VkPipelineLayout m_layout = VK_NULL_HANDLE;

		VkDescriptorSet		m_computeDescriptorSet = VK_NULL_HANDLE;

		friend class VulkanRenderDevice;
	};


}


