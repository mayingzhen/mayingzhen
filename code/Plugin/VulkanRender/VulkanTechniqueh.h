#pragma once


namespace ma
{

	class VulkanTechnique : public Technique
	{
	public:

		VulkanTechnique();

		~VulkanTechnique();

// 		virtual void		Bind();
// 
// 		virtual void		UnBind();

		virtual void		CommitChanges();

		void				RT_StreamComplete();

		void				SetTexture(Uniform* uniform, const SamplerState* sampler);

	private:

		VkDescriptorPool m_desc_pool;

		VkDescriptorSetLayout m_desc_layout[2];
		VkDescriptorSet m_descriptorSets[2];

		VkDescriptorSetLayout m_desc_layout_sampler[2];
		VkDescriptorSet m_descriptorSets_sampler[2];


		VkPipelineCache m_pipelineCache;

		VkPipeline m_pipeline;

		VkPipelineLayout m_pipelineLayout;

		bool m_bSamplerDirty = true;
	};


}


