#pragma once


namespace ma
{

	class VulkanShaderProgram : public ShaderProgram
	{
	public:

		VulkanShaderProgram();

		~VulkanShaderProgram();

		virtual void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		virtual void ParseUniform();

		virtual void RT_SetShader();

		virtual void CommitChanges();

		void Destory();

	private:
		void ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
			RefPtr<ConstantBuffer> ConstantBuffersPtr[]);

	private:
		std::vector< RefPtr<ConstantBuffer> > m_vecVSConstantBuffers;
		std::vector< RefPtr<ConstantBuffer> > m_vecPSConstantBuffers;

		VkPipelineShaderStageCreateInfo m_shaderStages[2];

		VkDescriptorSetLayout m_desc_layout;

		VkDescriptorPool m_desc_pool;

		std::vector<VkDescriptorSet> m_descriptorSets;

		VkPipelineCache m_pipelineCache;

		VkPipeline m_pipeline;

		VkPipelineLayout m_pipelineLayout;

		friend class VulkanRenderDevice;
	};


}


