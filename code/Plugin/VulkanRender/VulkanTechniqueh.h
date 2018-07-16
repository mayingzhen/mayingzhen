#pragma once


namespace ma
{
	class VulkanPipeline;

	class VulkanTechnique : public Technique
	{
	public:

		VulkanTechnique();

		~VulkanTechnique();

		virtual void		RT_StreamComplete();

		virtual void		RT_SetSampler(Uniform* pUniform, SamplerState* pSampler);

	private:
		
		void				UpdateUniformDescriptorSets();

		void				UpdateSamplerDescriptorSets();

	public:
		VkDescriptorSet		m_descriptorSets_uniform[2];

		VkDescriptorSet		m_descriptorSets_sampler[2];
	};


}


