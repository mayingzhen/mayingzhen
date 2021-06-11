#pragma once

#include "VulkanShaderUtil.h"
#include "VulkanConstantBuffer.h"

#include "glslang/glslang/Include/ResourceLimits.h"
#include "glslang/glslang/Public/ShaderLang.h"
#include "glslang/SPIRV/GlslangToSpv.h"

#include "SPIRV-Cross/spirv_glsl.hpp"
#include "SPIRV-Cross/spirv_hlsl.hpp"
#include "SPIRV-Cross/spirv_msl.hpp"
#include "Engine/Material/PrePareShaderSource.h"

namespace ma
{
	namespace VulkanShaderUtil
	{
		uint32_t m_cbshiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_texshiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_samplershiftBinding[ShaderType_Number] = { 0 };
		uint32_t m_uavshiftBinding[ShaderType_Number] = { 0 };

		void InitShiftBinding()
		{
			uint32_t offset = 0;
			for (uint32_t i = 0; i < ShaderType_Number; ++i)
			{
				m_cbshiftBinding[i] = offset;
				m_texshiftBinding[i] = m_cbshiftBinding[i] + MAX_SHADER_PARAMETER_GROUPS;
				m_samplershiftBinding[i] = m_texshiftBinding[i] + MAX_TEXTURE_UNITS;
				m_uavshiftBinding[i] = m_samplershiftBinding[i] + MAX_TEXTURE_UNITS;
				offset = m_uavshiftBinding[i] + MAX_TEXTURE_UNITS;
			}
		}

		uint32_t cbshiftBinding(ShaderType type)
		{
			return m_cbshiftBinding[type];
		}

		uint32_t texshiftBinding(ShaderType type)
		{
			return m_texshiftBinding[type];
		}

		uint32_t samplershiftBinding(ShaderType type)
		{
			return m_samplershiftBinding[type];
		}

		uint32_t uavshiftBinding(ShaderType type)
		{
			return m_uavshiftBinding[type];
		}

		VkShaderStageFlagBits ToVkShader(ShaderType type)
		{
			if (type == VS)
			{
				return VK_SHADER_STAGE_VERTEX_BIT;
			}
			else if (type == GS)
			{
				return VK_SHADER_STAGE_GEOMETRY_BIT;
			}
			else if (type == PS)
			{
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			}
			else if (type == CS)
			{
				return VK_SHADER_STAGE_COMPUTE_BIT;
			}
			else
			{
				ASSERT(false);
				return VK_SHADER_STAGE_VERTEX_BIT;
			}
		}

		EShLanguage FindLanguage(ShaderType eType)
		{
			switch (eType)
			{
			case VS:
				return EShLangVertex;
			case GS:
				return EShLangGeometry;
			case PS:
				return EShLangFragment;
			case CS:
				return EShLangCompute;
			default:
				ASSERT(false);
				return EShLangVertex;
			}
		}

		void init_resources(TBuiltInResource& Resources)
		{
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

		void BuildReflection(glslang::TProgram& program, ShaderType eType, UniformData& out)
		{
			program.buildReflection();
			//program.dumpReflection();

			int nNumUniformBlock = program.getNumLiveUniformBlocks();

			std::vector< VulkanConstantBuffer* > vecCB(nNumUniformBlock);

			for (int iBlock = 0; iBlock < nNumUniformBlock; ++iBlock)
			{
				const char* pszName = program.getUniformBlockName(iBlock);
				int size = program.getUniformBlockSize(iBlock);
				const glslang::TType* type = program.getUniformBlockTType(iBlock);
				int binding = type->getQualifier().layoutBinding;

				if (size == 0)
				{
					RefPtr<Uniform> pUniform = CreateUniform(pszName);
					pUniform->SetShaderType(eType);
					pUniform->SetIndex(binding);

					out.vecStorgeBuffer.emplace_back(pUniform);
				}
				else
				{
					RefPtr<VulkanConstantBuffer> pConstantBuffer = new VulkanConstantBuffer();
					out.vecConstBuffer.emplace_back(pConstantBuffer);

					pConstantBuffer->SetName(pszName);
					pConstantBuffer->SetBound(binding);
					pConstantBuffer->SetSize(size);

					vecCB[iBlock] = pConstantBuffer.get();
				}
			}

			int nNumUniformVar = program.getNumLiveUniformVariables();
			for (int iVar = 0; iVar < nNumUniformVar; ++iVar)
			{
				const char* pszName = program.getUniformName(iVar);
				int offset = program.getUniformBufferOffset(iVar);
				int arraySize = program.getUniformArraySize(iVar);
				int nBinding = program.getUniformBinding(iVar);
				const glslang::TType* type = program.getUniformTType(iVar);
				ASSERT(type);
				if (type == NULL)
					continue;

				if (type->isTexture())
				{
					RefPtr<Uniform> pUniform = CreateUniform(pszName);
					pUniform->SetShaderType(eType);
					pUniform->SetIndex(nBinding - m_texshiftBinding[eType]);
					out.vecSampler.emplace_back(pUniform);
					continue;
				}

				int size = 0;
				if (type->getBasicType() == glslang::EbtFloat ||
					type->getBasicType() == glslang::EbtInt)
				{
					if (type->getMatrixCols() * type->getMatrixRows() > 0)
					{
						size = sizeof(float) * type->getMatrixCols() * type->getMatrixRows() * arraySize;
					}
					else if (type->getVectorSize() > 0)
					{
						size = sizeof(float) * type->getVectorSize() * arraySize;
					}
				}
				int blockIndex = program.getUniformBlockIndex(iVar);
				if (blockIndex >= 0)
				{
					ASSERT(blockIndex < nNumUniformBlock);

					VulkanConstantBuffer* pConstantBuffer = vecCB[blockIndex];
					if (pConstantBuffer)
					{
						Uniform* pUniform = pConstantBuffer->AddUniform(pszName);
						pUniform->SetShaderType(eType);
						pUniform->SetIndex(-1);
						pUniform->SetOffset(offset);
						ASSERT(size > 0);
						pUniform->SetSize(size);
					}
				}
			}
		}

		void HlslToSpirv(const char* vshSource, uint32_t vshSize, const char* funName, ShaderType eType, 
			std::vector<uint32_t>& vtx_spv, UniformData& out)
		{
			//VS
			EShLanguage stage = FindLanguage(eType);
			glslang::TShader shader(stage);
			glslang::TProgram program;
			const char* shaderStrings[1];
			TBuiltInResource Resources;
			init_resources(Resources);

			// Enable SPIR-V and Vulkan rules when parsing GLSL
			EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgReadHlsl);

			shaderStrings[0] = vshSource;
			shader.setStrings(shaderStrings, 1);
			shader.setEntryPoint(funName);
			shader.setAutoMapBindings(true);
			shader.setHlslIoMapping(true);
			shader.setShiftCbufferBinding(m_cbshiftBinding[eType]);
			shader.setShiftTextureBinding(m_texshiftBinding[eType]);
			shader.setShiftSamplerBinding(m_samplershiftBinding[eType]);
			shader.setShiftUavBinding(m_uavshiftBinding[eType]);

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

			if (!program.mapIO())
			{
				LogError(shader.getInfoLog());
				LogError(shader.getInfoDebugLog());
				return;
			}

			glslang::GlslangToSpv(*program.getIntermediate(stage), vtx_spv);

			BuildReflection(program, eType, out);
		}


		void CreateShaderMode(const std::string& shaderFile, const std::string shaderMacro, ShaderType type,
			 VkPipelineShaderStageCreateInfo shaderStage, UniformData& out)
		{
			std::string strPath = GetRenderSystem()->GetShaderPath();

			std::vector<std::string> vecSplit = StringUtil::split(shaderFile, ":");
			ASSERT(vecSplit.size() == 2);
			std::string strPathFile = strPath + vecSplit[0];
			std::string strFunName = vecSplit[1];

			std::string strMacro = ReplaceDefines(shaderMacro.c_str());
			std::string strSource = PrePareShaderSource(strPathFile.c_str(), strMacro);

			vks::VulkanDevice* device = GetVulkanDevice();
			VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

			std::vector<uint32_t> vtx_spv;

			HlslToSpirv(strSource.c_str(), strSource.length(), strFunName.c_str(), type, vtx_spv, out);

			shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStage.pNext = NULL;
			shaderStage.pSpecializationInfo = NULL;
			shaderStage.flags = 0;
			shaderStage.stage = ToVkShader(type);
			shaderStage.pName = strFunName.c_str();

			VkShaderModuleCreateInfo moduleCreateInfo;
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.pNext = NULL;
			moduleCreateInfo.flags = 0;
			moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(uint32_t);
			moduleCreateInfo.pCode = vtx_spv.data();
			VK_CHECK_RESULT(vkCreateShaderModule(device->logicalDevice, &moduleCreateInfo, NULL, &shaderStage.module));
		}
	}
}