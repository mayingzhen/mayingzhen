#pragma once


namespace ma
{

	class VulkanPipeline : public Referenced
	{
	public:

		VulkanPipeline();

		~VulkanPipeline();


		void	RT_StreamComplete(VulkanTechnique* pTech);

	public:
		VkDescriptorPool m_desc_pool;

		VkDescriptorSetLayout m_desc_layout[2];

		VkDescriptorSetLayout m_desc_layout_sampler[2];

		VkPipelineCache m_pipelineCache;

		VkPipeline m_pipeline;

		VkPipelineLayout m_pipelineLayout;
	};

	RefPtr<VulkanPipeline> CreateVulkanPipeline(VulkanTechnique* pTech);

}


