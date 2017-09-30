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

		void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		void ParseShaderUniform(ShaderType eType, const vector<uint32>& vtx_spv);

		void HlslToSpirv(const char* vshSource, UINT vshSize, ShaderType eType, std::vector<UINT>& vtx_spv);

	public:
		std::vector<UINT> m_vtx_spv;
		std::vector<UINT> m_frg_spv;

		VkPipelineShaderStageCreateInfo m_shaderStages[2];

		friend class VulkanRenderDevice;
	};


}


