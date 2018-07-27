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

		void ParseShaderUniform(ShaderType eType, const vector<uint32_t>& vtx_spv);

		void HlslToSpirv(const char* vshSource, uint32_t vshSize, ShaderType eType, std::vector<uint32_t>& vtx_spv);

		void CreateShaderMode(const char* shSource, uint32_t shSize, ShaderType type);

		void CreatePipelineLayout();

		void CreatePipelineCache();

		void CreateDescriptorPool();

		void CreatePipeline();

	public:
		VkDescriptorPool m_desc_pool = VK_NULL_HANDLE;

		VkDescriptorSetLayout m_desc_layout;

		VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;

		VkPipeline m_pipeline = VK_NULL_HANDLE;

		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

		VkPipelineShaderStageCreateInfo m_shaderStages[ShaderType_Number];

		uint32_t m_cbshiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_texshiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_samplershiftBinding[ShaderType_Number] = { 0 };

		friend class VulkanRenderDevice;
	};


}


