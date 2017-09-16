#pragma once


namespace ma
{
	class VulkanPipeline;

	class VulkanTechnique : public Technique
	{
	public:

		VulkanTechnique();

		~VulkanTechnique();

		//virtual void		Bind();

		virtual void		CommitChanges();

		void				RT_StreamComplete();

		void				UpdateSamplerDescriptorSets();

	private:
		VkDescriptorSet m_descriptorSets[2];

		VkDescriptorSet m_descriptorSets_sampler[2];

		RefPtr<VulkanPipeline> m_pPipline;

		bool m_bSamperDirty = true;
	};


}


