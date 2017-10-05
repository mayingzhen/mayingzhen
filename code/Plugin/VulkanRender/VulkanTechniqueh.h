#pragma once


namespace ma
{
	class VulkanPipeline;

	class VulkanTechnique : public Technique
	{
	public:

		VulkanTechnique();

		~VulkanTechnique();

		void				RT_StreamComplete();

		void				UpdateSamplerDescriptorSets();

	public:
		VkDescriptorSet m_descriptorSets[2];

		VkDescriptorSet m_descriptorSets_sampler[2];

		RefPtr<VulkanPipeline> m_pPipline;

		bool m_bSamperDirty = true;
	};


}


