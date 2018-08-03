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

		void HlslToSpirv(const char* vshSource, uint32_t vshSize, const char* funName, ShaderType eType, std::vector<uint32_t>& vtx_spv);

		void CreateShaderMode(const std::string& shaderFile, ShaderType type);

		void CreatePipelineLayout();

		void CreatePipelineCache();

		void CreateDescriptorPool();

		void CreateGraphicsPipeline();

		void CreateComputePipeline();

	public:
		VkDescriptorPool m_desc_pool = VK_NULL_HANDLE;

		VkDescriptorSetLayout m_desc_layout;

		struct Pipeline
		{
			VkPipelineCache _Cache = VK_NULL_HANDLE;

			VkPipeline _Pipeline = VK_NULL_HANDLE;

			VkPipelineLayout _Layout = VK_NULL_HANDLE;
		};

		Pipeline m_graphicPip;

		Pipeline m_computePip;

		VkPipelineShaderStageCreateInfo m_shaderStages[ShaderType_Number];

		std::string	m_strFunName[ShaderType_Number];

		uint32_t m_cbshiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_texshiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_samplershiftBinding[ShaderType_Number] = { 0 };

		friend class VulkanRenderDevice;
	};


}


