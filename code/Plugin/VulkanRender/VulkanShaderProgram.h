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

	public:
		std::vector<uint32_t> m_vtx_spv;
		std::vector<uint32_t> m_frg_spv;

		VkPipelineShaderStageCreateInfo m_shaderStages[2];

		friend class VulkanRenderDevice;
	};


}


