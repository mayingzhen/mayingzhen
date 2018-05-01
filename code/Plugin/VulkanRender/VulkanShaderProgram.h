#pragma once


namespace ma
{

	class VulkanShaderProgram : public ShaderProgram
	{
	public:

		VulkanShaderProgram();

		~VulkanShaderProgram();

	private:
		virtual void RT_StreamComplete();

		void Destory();

		void CreateFromSource(const char* vshSource, uint32_t vshSize, const char* fshSource, uint32_t fshSize);

		void ParseShaderUniform(ShaderType eType, const vector<uint32_t>& vtx_spv);

		void HlslToSpirv(const char* vshSource, uint32_t vshSize, ShaderType eType, std::vector<uint32_t>& vtx_spv);

		void CreatePipeline();

	public:
		VkDescriptorPool m_desc_pool;

		VkDescriptorSetLayout m_desc_layout_uniform[2];

		VkDescriptorSetLayout m_desc_layout_sampler[2];

		VkPipelineCache m_pipelineCache;

		VkPipeline m_pipeline;

		VkPipelineLayout m_pipelineLayout;

		VkPipelineShaderStageCreateInfo m_shaderStages[2];

		friend class VulkanRenderDevice;
	};


}


