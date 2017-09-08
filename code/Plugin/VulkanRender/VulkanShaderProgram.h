#pragma once


namespace ma
{

	class VulkanShaderProgram : public ShaderProgram
	{
	public:

		VulkanShaderProgram();

		~VulkanShaderProgram();

		virtual void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		virtual void RT_SetShader();

		virtual void CommitChanges();

		void Destory();

	private:
		void ParseShaderUniform(ShaderType eType, const vector<uint32>& vtx_spv);

		void HlslToSpirv(const char* vshSource, UINT vshSize, ShaderType eType, std::vector<UINT>& vtx_spv);

	public:
		std::vector< RefPtr<ConstantBuffer> > m_vecVSConstantBuffers;
		std::vector< RefPtr<ConstantBuffer> > m_vecPSConstantBuffers;

		std::vector< Uniform* > m_vecPSSamplers;

		VkPipelineShaderStageCreateInfo m_shaderStages[2];

		friend class VulkanRenderDevice;
	};


}


