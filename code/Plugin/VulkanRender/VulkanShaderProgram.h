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

		VkPipelineShaderStageCreateInfo CreateShaderMode(const std::string& shaderFile, ShaderType type);

		void CreatePipelineLayout();

		void CreatePipelineCache();

		void CreateDescriptorPool();

		void CreateGraphicsPipeline();

		void CreateComputePipeline(const VkPipelineShaderStageCreateInfo& stage);

	public:
		VkDescriptorPool m_desc_pool = VK_NULL_HANDLE;

		

		struct Pipeline
		{
			VkDescriptorSetLayout m_desc_layout = VK_NULL_HANDLE;

			VkPipelineCache _Cache = VK_NULL_HANDLE;

			VkPipeline _Pipeline = VK_NULL_HANDLE;

			VkPipelineLayout _Layout = VK_NULL_HANDLE;
		};

		Pipeline m_graphicPip;

		Pipeline m_computePip;

		std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;

		std::string	m_strFunName[ShaderType_Number];

		uint32_t m_cbshiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_texshiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_samplershiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_uavshiftBinding[ShaderType_Number] = { 0 };

		friend class VulkanRenderDevice;
	};


}


