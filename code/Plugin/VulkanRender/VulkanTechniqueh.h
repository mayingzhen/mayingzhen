#pragma once


namespace ma
{
	class VulkanPipeline;

	class VulkanTechnique : public Technique
	{
	public:

		VulkanTechnique();

		virtual ~VulkanTechnique();

		virtual void		RT_StreamComplete();

		virtual void		RT_SetSampler(Uniform* pUniform, SamplerState* pSampler);

		virtual void		RT_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer);

	private:
		
		void				UpdateGrapicUniformDescriptorSets(ShaderType eType);

		void				UpdateGrapicSamplerDescriptorSets(ShaderType eType,Uniform* pUniform, SamplerState* pSampler);

	public:

		VkDescriptorSet		m_grapicDescriptorSet = VK_NULL_HANDLE;
	};


	class VulkanComputeTechnique : public ComputeTechnique
	{
	public:

		VulkanComputeTechnique();

		virtual ~VulkanComputeTechnique();

		virtual void		RT_StreamComplete();

		virtual void		RT_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer);

	private:
		void				UpdateComputeUniformDescriptorSets();

		void				UpdateComputeStogeBufferDescriptorSets(Uniform* pUniform, HardwareBuffer* pBuffer);

	public:

		VkDescriptorSet		m_computeDescriptorSet = VK_NULL_HANDLE;
	};



}


