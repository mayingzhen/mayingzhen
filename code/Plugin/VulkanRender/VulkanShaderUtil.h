#pragma once


namespace ma
{
	struct UniformData
	{
		std::vector< RefPtr<VulkanConstantBuffer> > vecConstBuffer;
		std::vector< RefPtr<Uniform> > vecSampler;
		std::vector< RefPtr<Uniform> > vecStorgeBuffer;
	};

	namespace VulkanShaderUtil
	{
		void InitShiftBinding();

		void CreateShaderMode(const std::string& shaderFile, const std::string shaderMacro, ShaderType type,
			VkPipelineShaderStageCreateInfo& vkShaderStage, UniformData& uniformData);

		uint32_t cbshiftBinding(ShaderType type);
		uint32_t texshiftBinding(ShaderType type);
		uint32_t samplershiftBinding(ShaderType type);
		uint32_t uavshiftBinding(ShaderType type);
	};
}