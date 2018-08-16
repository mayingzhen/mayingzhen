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
		
		void				UpdateGrapicUniformDescriptorSets(ShaderType eType);

		void				UpdateGrapicSamplerDescriptorSets(ShaderType eType);

		void				UpdateComputeUniformDescriptorSets();

		void				UpdateComputeStogeBufferDescriptorSets(Uniform* pUniform, HardwareBuffer* pBuffer);

	public:

		VkDescriptorSet		m_grapicDescriptorSet;

		VkDescriptorSet		m_computeDescriptorSet;
	};


}


