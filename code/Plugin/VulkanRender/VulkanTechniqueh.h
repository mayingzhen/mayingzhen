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

		virtual void		RT_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer);

	private:
		
		void				UpdateUniformDescriptorSets(ShaderType eType);

		void				UpdateSamplerDescriptorSets(ShaderType eType);

		void				UpdateComputeDescriptorSets(HardwareBuffer* pBuffer);

	public:

		VkDescriptorSet		m_descriptorSet;
	};


}


