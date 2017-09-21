#include "VulkanShaderProgram.h"
#include "VulkanConstantBuffer.h"
#include "..\glslang\glslang\Include\ResourceLimits.h"
#include "..\glslang\glslang\Public\ShaderLang.h"
#include "..\glslang\SPIRV\GlslangToSpv.h"

#include "..\SPIRV-Cross\spirv_glsl.hpp"
#include "..\..\..\SPIRV-Cross\spirv_hlsl.hpp"
#include "..\..\..\SPIRV-Cross\spirv_msl.hpp"
#include "VulkanRenderState.h"
#include "Engine\Material\PrePareShaderSource.h"

namespace ma
{

	VulkanShaderProgram::VulkanShaderProgram()
	{
	}

	VulkanShaderProgram::~VulkanShaderProgram()
	{
		Destory();
	}

	void VulkanShaderProgram::Destory()
	{

	}

	void init_resources(TBuiltInResource &Resources) {
		Resources.maxLights = 32;
		Resources.maxClipPlanes = 6;
		Resources.maxTextureUnits = 32;
		Resources.maxTextureCoords = 32;
		Resources.maxVertexAttribs = 64;
		Resources.maxVertexUniformComponents = 4096;
		Resources.maxVaryingFloats = 64;
		Resources.maxVertexTextureImageUnits = 32;
		Resources.maxCombinedTextureImageUnits = 80;
		Resources.maxTextureImageUnits = 32;
		Resources.maxFragmentUniformComponents = 4096;
		Resources.maxDrawBuffers = 32;
		Resources.maxVertexUniformVectors = 128;
		Resources.maxVaryingVectors = 8;
		Resources.maxFragmentUniformVectors = 16;
		Resources.maxVertexOutputVectors = 16;
		Resources.maxFragmentInputVectors = 15;
		Resources.minProgramTexelOffset = -8;
		Resources.maxProgramTexelOffset = 7;
		Resources.maxClipDistances = 8;
		Resources.maxComputeWorkGroupCountX = 65535;
		Resources.maxComputeWorkGroupCountY = 65535;
		Resources.maxComputeWorkGroupCountZ = 65535;
		Resources.maxComputeWorkGroupSizeX = 1024;
		Resources.maxComputeWorkGroupSizeY = 1024;
		Resources.maxComputeWorkGroupSizeZ = 64;
		Resources.maxComputeUniformComponents = 1024;
		Resources.maxComputeTextureImageUnits = 16;
		Resources.maxComputeImageUniforms = 8;
		Resources.maxComputeAtomicCounters = 8;
		Resources.maxComputeAtomicCounterBuffers = 1;
		Resources.maxVaryingComponents = 60;
		Resources.maxVertexOutputComponents = 64;
		Resources.maxGeometryInputComponents = 64;
		Resources.maxGeometryOutputComponents = 128;
		Resources.maxFragmentInputComponents = 128;
		Resources.maxImageUnits = 8;
		Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		Resources.maxCombinedShaderOutputResources = 8;
		Resources.maxImageSamples = 0;
		Resources.maxVertexImageUniforms = 0;
		Resources.maxTessControlImageUniforms = 0;
		Resources.maxTessEvaluationImageUniforms = 0;
		Resources.maxGeometryImageUniforms = 0;
		Resources.maxFragmentImageUniforms = 8;
		Resources.maxCombinedImageUniforms = 8;
		Resources.maxGeometryTextureImageUnits = 16;
		Resources.maxGeometryOutputVertices = 256;
		Resources.maxGeometryTotalOutputComponents = 1024;
		Resources.maxGeometryUniformComponents = 1024;
		Resources.maxGeometryVaryingComponents = 64;
		Resources.maxTessControlInputComponents = 128;
		Resources.maxTessControlOutputComponents = 128;
		Resources.maxTessControlTextureImageUnits = 16;
		Resources.maxTessControlUniformComponents = 1024;
		Resources.maxTessControlTotalOutputComponents = 4096;
		Resources.maxTessEvaluationInputComponents = 128;
		Resources.maxTessEvaluationOutputComponents = 128;
		Resources.maxTessEvaluationTextureImageUnits = 16;
		Resources.maxTessEvaluationUniformComponents = 1024;
		Resources.maxTessPatchComponents = 120;
		Resources.maxPatchVertices = 32;
		Resources.maxTessGenLevel = 64;
		Resources.maxViewports = 16;
		Resources.maxVertexAtomicCounters = 0;
		Resources.maxTessControlAtomicCounters = 0;
		Resources.maxTessEvaluationAtomicCounters = 0;
		Resources.maxGeometryAtomicCounters = 0;
		Resources.maxFragmentAtomicCounters = 8;
		Resources.maxCombinedAtomicCounters = 8;
		Resources.maxAtomicCounterBindings = 1;
		Resources.maxVertexAtomicCounterBuffers = 0;
		Resources.maxTessControlAtomicCounterBuffers = 0;
		Resources.maxTessEvaluationAtomicCounterBuffers = 0;
		Resources.maxGeometryAtomicCounterBuffers = 0;
		Resources.maxFragmentAtomicCounterBuffers = 1;
		Resources.maxCombinedAtomicCounterBuffers = 1;
		Resources.maxAtomicCounterBufferSize = 16384;
		Resources.maxTransformFeedbackBuffers = 4;
		Resources.maxTransformFeedbackInterleavedComponents = 64;
		Resources.maxCullDistances = 8;
		Resources.maxCombinedClipAndCullDistances = 8;
		Resources.maxSamples = 4;
		Resources.limits.nonInductiveForLoops = 1;
		Resources.limits.whileLoops = 1;
		Resources.limits.doWhileLoops = 1;
		Resources.limits.generalUniformIndexing = 1;
		Resources.limits.generalAttributeMatrixVectorIndexing = 1;
		Resources.limits.generalVaryingIndexing = 1;
		Resources.limits.generalSamplerIndexing = 1;
		Resources.limits.generalVariableIndexing = 1;
		Resources.limits.generalConstantMatrixVectorIndexing = 1;
	}

	EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type) {
		switch (shader_type) {
		case VK_SHADER_STAGE_VERTEX_BIT:
			return EShLangVertex;

		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			return EShLangTessControl;

		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			return EShLangTessEvaluation;

		case VK_SHADER_STAGE_GEOMETRY_BIT:
			return EShLangGeometry;

		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return EShLangFragment;

		case VK_SHADER_STAGE_COMPUTE_BIT:
			return EShLangCompute;

		default:
			return EShLangVertex;
		}
	}


	void VulkanShaderProgram::HlslToSpirv(const char* vshSource, UINT vshSize, ShaderType eType, std::vector<UINT>& vtx_spv)
	{
		VkShaderStageFlagBits shaderStage = eType == VS ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;

		//VS
		EShLanguage stage = FindLanguage(shaderStage);
		glslang::TShader shader(stage);
		glslang::TProgram program;
		const char *shaderStrings[1];
		TBuiltInResource Resources;
		init_resources(Resources);

		// Enable SPIR-V and Vulkan rules when parsing GLSL
		EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgReadHlsl);

		shaderStrings[0] = vshSource;
		shader.setStrings(shaderStrings, 1);
		shader.setEntryPoint("main");

		if ( !shader.parse(&Resources, 100, false, messages) ) 
		{
			LogError(shader.getInfoLog());
			LogError(shader.getInfoDebugLog());
			return;  
		}

		program.addShader(&shader);

		if ( !program.link(messages) ) 
		{
			LogError(shader.getInfoLog());
			LogError(shader.getInfoDebugLog());
			return;
		}

		std::vector<UINT> vtx_spv_tem;

		glslang::GlslangToSpv(*program.getIntermediate(stage), vtx_spv_tem);

		{
			spirv_cross::CompilerGLSL glsl(vtx_spv_tem.data(), vtx_spv_tem.size());

			// The SPIR-V is now parsed, and we can perform reflection on it.
			spirv_cross::ShaderResources resources = glsl.get_shader_resources();

			for (auto &resource : resources.uniform_buffers)
			{
				unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
				glsl.set_decoration(resource.id, spv::DecorationDescriptorSet, eType == VS ? 0 : 1);
			}

			// Get all sampled images in the shader.
			for (auto &resource : resources.separate_samplers)
			{
				unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
				glsl.set_decoration(resource.id, spv::DecorationDescriptorSet, 2);
			}

			for (auto &resource : resources.separate_images)
			{
				unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
				glsl.set_decoration(resource.id, spv::DecorationDescriptorSet, 3);
			}

			// Set some options.
			spirv_cross::CompilerGLSL::Options options;
			options.vulkan_semantics = true;
			glsl.set_options(options);

			// Compile to GLSL, ready to give to GL driver.
			std::string source = glsl.compile();

			EShLanguage stage = FindLanguage(shaderStage);
			glslang::TShader shader(stage);
			glslang::TProgram program;
			const char *shaderStrings[1];
			TBuiltInResource Resources;
			init_resources(Resources);

			// Enable SPIR-V and Vulkan rules when parsing GLSL
			EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

			shaderStrings[0] = source.c_str();
			shader.setStrings(shaderStrings, 1);
			shader.setEntryPoint("main");

			if (!shader.parse(&Resources, 100, false, messages))
			{
				LogError(shader.getInfoLog());
				LogError(shader.getInfoDebugLog());
				return;
			}

			program.addShader(&shader);

			if (!program.link(messages))
			{
				LogError(shader.getInfoLog());
				LogError(shader.getInfoDebugLog());
				return;
			}

			if (0)
			{
				program.buildReflection();
				program.dumpReflection();
			}

			glslang::GlslangToSpv(*program.getIntermediate(stage), vtx_spv);

		}
	}

	void VulkanShaderProgram::CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize)
	{
		Destory();

	
		vks::VulkanDevice* device = GetVulkanDevice();
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		//VS
		{
			std::vector<unsigned int> vtx_spv;
			m_shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			m_shaderStages[0].pNext = NULL;
			m_shaderStages[0].pSpecializationInfo = NULL;
			m_shaderStages[0].flags = 0;
			m_shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
			m_shaderStages[0].pName = "main";

			HlslToSpirv(vshSource, vshSize, VS, vtx_spv);

			ParseShaderUniform(VS, vtx_spv);

			VkShaderModuleCreateInfo moduleCreateInfo;
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.pNext = NULL;
			moduleCreateInfo.flags = 0;
			moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(UINT);
			moduleCreateInfo.pCode = vtx_spv.data();
			VkResult res = vkCreateShaderModule(device->logicalDevice, &moduleCreateInfo, NULL, &m_shaderStages[0].module);
			assert(res == VK_SUCCESS);
		}

		// PS
		{
			std::vector<unsigned int> fsh_spv;
			m_shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			m_shaderStages[1].pNext = NULL;
			m_shaderStages[1].pSpecializationInfo = NULL;
			m_shaderStages[1].flags = 0;
			m_shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			m_shaderStages[1].pName = "main";

			HlslToSpirv(fshSource, fshSize, PS, fsh_spv);

			ParseShaderUniform(PS, fsh_spv);

			VkShaderModuleCreateInfo moduleCreateInfo;
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.pNext = NULL;
			moduleCreateInfo.flags = 0;
			moduleCreateInfo.codeSize = fsh_spv.size() * sizeof(unsigned int);
			moduleCreateInfo.pCode = fsh_spv.data();
			VkResult res = vkCreateShaderModule(device->logicalDevice, &moduleCreateInfo, NULL, &m_shaderStages[1].module);
			assert(res == VK_SUCCESS);
		}

		return;
	}


	void VulkanShaderProgram::ParseShaderUniform(ShaderType eType,const vector<uint32>& vtx_spv)
	{
		spirv_cross::CompilerGLSL glsl(vtx_spv.data(),vtx_spv.size());

		if (0)
		{
			// Set some options.
			spirv_cross::CompilerGLSL::Options options;
			options.vulkan_semantics = true;
			glsl.set_options(options);

			// Compile to GLSL, ready to give to GL driver.
			std::string source = glsl.compile();

			LogInfo("%s", source.c_str());
		}

		spirv_cross::ShaderResources resources = glsl.get_shader_resources();
		for (auto &resource : resources.uniform_buffers)
		{
			const spirv_cross::SPIRType& spType = glsl.get_type(resource.type_id);
			size_t size_ = glsl.get_declared_struct_size(spType);
			unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
			RefPtr<VulkanConstantBuffer> pConstantBuffer = new VulkanConstantBuffer();
			pConstantBuffer->SetName(resource.name.c_str());
			pConstantBuffer->SetBound(binding);
			pConstantBuffer->SetSize(size_);
			this->AddConstBuffer(eType, pConstantBuffer.get());
			for (UINT i = 0; i < spType.member_types.size(); ++i)
			{
				std::string str = glsl.get_member_name(spType.self, i);
				size_t offset = glsl.type_struct_member_offset(spType, i);
				size_t size = glsl.get_declared_struct_member_size(spType, i);

				Uniform* pUniform = pConstantBuffer->AddUniform(str.c_str());
				pUniform->SetIndex(i);
				pUniform->SetOffset(offset);
				pUniform->SetSize(size);
			}
		}

		for (auto &resource : resources.separate_images)
		{
			unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);

			RefPtr<Uniform> pUniform = CreateUniform(resource.name.c_str());
			pUniform->SetIndex(binding);

			this->AddSampler(pUniform.get());
		}
	}

	void VulkanShaderProgram::RT_SetShader()
	{
	}

	void VulkanShaderProgram::CommitChanges()
	{
// 		for (UINT i = 0; i < m_vecVSConstantBuffers.size(); ++i)
// 		{
// 			m_vecVSConstantBuffers[i]->Apply();
// 		}
// 
// 		for (UINT i = 0; i < m_vecPSConstantBuffers.size(); ++i)
// 		{
// 			m_vecPSConstantBuffers[i]->Apply();
// 		}
	}

	void VulkanShaderProgram::RT_StreamComplete()
	{
		ASSERT(GetResState() == ResLoaded);

		std::string strPath = GetRenderSystem()->GetShaderPath();

		std::string strPathVS = strPath + GetVSFile() + ".vert";
		std::string strPathFS = strPath + GetPSFile() + ".frag";

		std::string strVshSource = PrePareShaderSource(strPathVS.c_str(), GetShaderMacro());
		std::string strFshSource = PrePareShaderSource(strPathFS.c_str(), GetShaderMacro());

		CreateFromSource(strVshSource.c_str(), strVshSource.length(),
			strFshSource.c_str(), strFshSource.length());

		SetResState(ResInited);
	}

}

