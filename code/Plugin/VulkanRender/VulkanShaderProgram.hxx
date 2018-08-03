#include "VulkanShaderProgram.h"
#include "VulkanConstantBuffer.h"
#include "VulkanRenderPass.h"
#include "glslang/glslang/Include/ResourceLimits.h"
#include "glslang/glslang/Public/ShaderLang.h"
#include "glslang/SPIRV/GlslangToSpv.h"

#include "SPIRV-Cross/spirv_glsl.hpp"
#include "SPIRV-Cross/spirv_hlsl.hpp"
#include "SPIRV-Cross/spirv_msl.hpp"
#include "VulkanRenderState.h"
#include "Engine/Material/PrePareShaderSource.h"


namespace ma
{

	VulkanShaderProgram::VulkanShaderProgram()
	{
		uint32_t offset = 0;
		for (uint32_t i = 0; i < ShaderType_Number; ++i)
		{
			m_cbshiftBinding[i] = offset;
			m_texshiftBinding[i] = m_cbshiftBinding[i] + MAX_SHADER_PARAMETER_GROUPS;
			m_samplershiftBinding[i] = m_texshiftBinding[i] + MAX_TEXTURE_UNITS;
			offset = m_samplershiftBinding[i] + MAX_TEXTURE_UNITS;
		}
	}

	VulkanShaderProgram::~VulkanShaderProgram()
	{
		Destory();
	}

	void VulkanShaderProgram::Destory()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		if (m_graphicPip._Pipeline)
		{
			vkDestroyPipeline(device->logicalDevice, m_graphicPip._Pipeline, NULL);
		}

		if (m_graphicPip._Cache)
		{
			vkDestroyPipelineCache(device->logicalDevice, m_graphicPip._Cache, NULL);
		}

		if (m_desc_pool)
		{
			vkDestroyDescriptorPool(device->logicalDevice, m_desc_pool, NULL);
		}
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

	EShLanguage FindLanguage(ShaderType eType)
	{
		switch (eType)
		{
		case VS:
			return EShLangVertex;
		case PS:
			return EShLangFragment;
		case CS:
			return EShLangCompute;
		default:
			ASSERT(false);
			return EShLangVertex;
		}
	}


	void VulkanShaderProgram::HlslToSpirv(const char* vshSource, uint32_t vshSize, const char* funName, ShaderType eType, std::vector<uint32_t>& vtx_spv)
	{
		//VS
		EShLanguage stage = FindLanguage(eType);
		glslang::TShader shader(stage);
		glslang::TProgram program;
		const char *shaderStrings[1];
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

		/*
		program.buildReflection();
		program.dumpReflection();

		int nNumUniformBlock = program.getNumLiveUniformBlocks();

		std::vector<VulkanConstantBuffer*> vecCB(nNumUniformBlock);

		for (int iBlock = 0; iBlock < nNumUniformBlock; ++iBlock)
		{
			const char* pszName = program.getUniformBlockName(iBlock);
			int size = program.getUniformBlockSize(iBlock);
			const glslang::TType* type = program.getUniformBlockTType(iBlock);
			int binding = type->getQualifier().layoutBinding;

			RefPtr<VulkanConstantBuffer> pConstantBuffer = new VulkanConstantBuffer();
			this->AddConstBuffer(eType, pConstantBuffer.get());

			pConstantBuffer->SetName(pszName);
			pConstantBuffer->SetBound(binding);
			pConstantBuffer->SetSize(size);

			vecCB[iBlock] = pConstantBuffer.get();
		}

		int nNumUniformVar = program.getNumLiveUniformVariables();
		for (int iVar = 0; iVar < nNumUniformVar; ++iVar)
		{
			const char* pszName = program.getUniformName(iVar);
			int offset = program.getUniformBufferOffset(iVar);
			int arraySize = program.getUniformArraySize(iVar);
			const glslang::TType* type = program.getUniformTType(iVar);
			int size = 0;
			if (type->getBasicType() == glslang::EbtFloat)
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

				Uniform* pUniform = pConstantBuffer->AddUniform(pszName);
				pUniform->SetIndex(-1);
				pUniform->SetOffset(offset);
				pUniform->SetSize(size);
			}
		}
		*/
	}

	VkShaderStageFlagBits ToVkShader(ShaderType type)
	{
		if (type == VS)
		{
			return VK_SHADER_STAGE_VERTEX_BIT;
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

	void VulkanShaderProgram::CreateShaderMode(const std::string& shaderFile, ShaderType type)
	{
		std::string strPath = GetRenderSystem()->GetShaderPath();

		const ShaderCreateInfo& info = this->GetShaderCreateInfo();

		std::vector<std::string> vecSplit = StringUtil::split(shaderFile, ":");
		ASSERT(vecSplit.size() == 2);
		std::string strPathFile = strPath + vecSplit[0];
		m_strFunName[type] = vecSplit[1];

		std::string strSource = PrePareShaderSource(strPathFile.c_str(), info.m_shaderMacro.c_str());

		vks::VulkanDevice* device = GetVulkanDevice();
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		std::vector<uint32_t> vtx_spv;
		m_shaderStages[type].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStages[type].pNext = NULL;
		m_shaderStages[type].pSpecializationInfo = NULL;
		m_shaderStages[type].flags = 0;
		m_shaderStages[type].stage = ToVkShader(type);
		m_shaderStages[type].pName = m_strFunName[type].c_str();

		HlslToSpirv(strSource.c_str(), strSource.length(), m_strFunName[type].c_str(), type, vtx_spv);

		ParseShaderUniform(type, vtx_spv);

		VkShaderModuleCreateInfo moduleCreateInfo;
		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfo.pNext = NULL;
		moduleCreateInfo.flags = 0;
		moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(uint32_t);
		moduleCreateInfo.pCode = vtx_spv.data();
		VK_CHECK_RESULT(vkCreateShaderModule(device->logicalDevice, &moduleCreateInfo, NULL, &m_shaderStages[type].module));
	}

	void VulkanShaderProgram::CreatePipelineLayout()
	{
		vks::VulkanDevice* device = GetVulkanDevice();
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		{
			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;

			for (uint32_t icb = 0; icb < MAX_SHADER_PARAMETER_GROUPS; ++icb)
			{
				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					VK_SHADER_STAGE_VERTEX_BIT,
					m_cbshiftBinding[VS] + icb));

				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					m_cbshiftBinding[PS] + icb));
			}

			for (uint32_t itex = 0; itex < MAX_TEXTURE_UNITS; ++itex)
			{
				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
					VK_SHADER_STAGE_VERTEX_BIT,
					m_texshiftBinding[VS] + itex));

				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					m_texshiftBinding[PS] + itex));

				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_SAMPLER,
					VK_SHADER_STAGE_VERTEX_BIT,
					m_samplershiftBinding[VS] + itex));

				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_SAMPLER,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					m_samplershiftBinding[PS] + itex));
			}


			VkDescriptorSetLayoutCreateInfo descriptorLayout;
			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
				setLayoutBindings.data(),
				static_cast<uint32_t>(setLayoutBindings.size()));
			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout));
		}

// 		{
// 			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
// 			for (uint32_t i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
// 			{
// 				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
// 					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 					VK_SHADER_STAGE_VERTEX_BIT,
// 					m_cbshiftBinding[VS] + i ) );
// 			}
// 
// 			VkDescriptorSetLayoutCreateInfo descriptorLayout;
// 			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
// 				setLayoutBindings.data(),
// 				static_cast<uint32_t>(setLayoutBindings.size()));
// 			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout_uniform[VS]));
// 		}

// 		{
// 			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
// 			for (uint32_t i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
// 			{
// 				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
// 					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 					VK_SHADER_STAGE_FRAGMENT_BIT,
// 					m_cbshiftBinding[PS] + i ) );
// 			}
// 
// 			VkDescriptorSetLayoutCreateInfo descriptorLayout;
// 			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
// 				setLayoutBindings.data(),
// 				static_cast<uint32_t>(setLayoutBindings.size()));
// 
// 			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout_uniform[PS]));
// 		}

// 		{
// 			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
// 			for (uint32_t i = 0; i < MAX_TEXTURE_UNITS/*pShader->GetSamplerCount()*/; ++i)
// 			{
// 				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
// 					VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
// 					VK_SHADER_STAGE_VERTEX_BIT,
// 					m_texshiftBinding[VS] + i ) );
// 			}
// 
// 			VkDescriptorSetLayoutCreateInfo descriptorLayout;
// 			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
// 				setLayoutBindings.data(),
// 				static_cast<uint32_t>(setLayoutBindings.size()));
// 
// 			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout_sampler[VS]));
// 		}

// 		{
// 			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
// 			for (uint32_t i = 0; i < MAX_TEXTURE_UNITS/*pShader->GetSamplerCount()*/; ++i)
// 			{
// 				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
// 					VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
// 					VK_SHADER_STAGE_FRAGMENT_BIT,
// 					m_texshiftBinding[PS] + i ) );
// 			}
// 
// 			VkDescriptorSetLayoutCreateInfo descriptorLayout;
// 			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
// 				setLayoutBindings.data(),
// 				static_cast<uint32_t>(setLayoutBindings.size()));
// 
// 			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout_sampler[PS]));
// 		}

// 		VkDescriptorSetLayout SetLayouts[4];
// 		SetLayouts[0] = m_desc_layout_uniform[VS];
// 		SetLayouts[1] = m_desc_layout_uniform[PS];
// 		SetLayouts[2] = m_desc_layout_sampler[VS];
// 		SetLayouts[3] = m_desc_layout_sampler[PS];

		/* Now use the descriptor layout to create a pipeline layout */
		VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
		pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pPipelineLayoutCreateInfo.pNext = NULL;
		pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;// pPushConstantRanges.size();
		pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;// pPushConstantRanges.data();
		pPipelineLayoutCreateInfo.setLayoutCount = 1;
		pPipelineLayoutCreateInfo.pSetLayouts = &m_desc_layout;

		VK_CHECK_RESULT(vkCreatePipelineLayout(device->logicalDevice, &pPipelineLayoutCreateInfo, NULL, &m_graphicPip._Layout));
	}

	void VulkanShaderProgram::CreatePipelineCache()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		VkPipelineCacheCreateInfo pipelineCache;
		pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		pipelineCache.pNext = NULL;
		pipelineCache.initialDataSize = 0;
		pipelineCache.pInitialData = NULL;
		pipelineCache.flags = 0;
		VK_CHECK_RESULT(vkCreatePipelineCache(device->logicalDevice, &pipelineCache, NULL, &m_graphicPip._Cache));
	}

	void VulkanShaderProgram::CreateDescriptorPool()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		// init_descriptor_pool
		VkDescriptorPoolSize type_count[3];
		type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		type_count[0].descriptorCount = MAX_SHADER_PARAMETER_GROUPS * 2 * 1000;

		type_count[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
		type_count[1].descriptorCount = 16 * 1000;

		type_count[2].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		type_count[2].descriptorCount = 16 * 1000;

		VkDescriptorPoolCreateInfo descriptor_pool = {};
		descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptor_pool.pNext = NULL;
		descriptor_pool.maxSets = (MAX_SHADER_PARAMETER_GROUPS * 2 + 32) * 1000;
		descriptor_pool.poolSizeCount = 3;
		descriptor_pool.pPoolSizes = type_count;

		VK_CHECK_RESULT(vkCreateDescriptorPool(device->logicalDevice, &descriptor_pool, NULL, &m_desc_pool));
	}


	void VulkanShaderProgram::CreateGraphicsPipeline()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		const ShaderCreateInfo& info = this->GetShaderCreateInfo();

		VulkanBlendStateObject bs;
		VulkanBlendStateObject* pVulkanBS = (VulkanBlendStateObject*)(info.m_pBlendState.get());
		if (pVulkanBS)
		{
			pVulkanBS->RT_StreamComplete();
		}
		else
		{
			pVulkanBS = &bs;
		}

		pVulkanBS->m_cb.attachmentCount = GetRenderPass()->m_arrColor.size();

		VulkanDepthStencilStateObject ds;
		VulkanDepthStencilStateObject* pVulkanDS = (VulkanDepthStencilStateObject*)(info.m_pDSState.get());
		if (pVulkanDS)
		{
			pVulkanDS->RT_StreamComplete();
		}
		else
		{
			pVulkanDS = &ds;
		}

		VulkanRasterizerStateObject rs;
		VulkanRasterizerStateObject* pVulkanRS = (VulkanRasterizerStateObject*)(info.m_pRSState.get());
		if (pVulkanRS)
		{
			pVulkanRS->RT_StreamComplete();
		}
		else
		{
			pVulkanRS = &rs;
		}

		VulkanVertexDeclaration* pVertexDec = (VulkanVertexDeclaration*)(info.m_pVertexDecl.get());
		ASSERT(pVertexDec);
		if (pVertexDec->m_attributeDescriptions.empty())
		{
			pVertexDec->RT_StreamComplete();
		}

		VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.pNext = NULL;
		dynamicState.pDynamicStates = dynamicStateEnables;
		dynamicState.dynamicStateCount = 0;

		VkPipelineInputAssemblyStateCreateInfo ia;
		ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		ia.pNext = NULL;
		ia.flags = 0;
		ia.primitiveRestartEnable = VK_FALSE;
		ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo vp = {};
		vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		vp.pNext = NULL;
		vp.flags = 0;
#ifndef __ANDROID__
		vp.viewportCount = 1/*NUM_VIEWPORTS*/;
		dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
		vp.scissorCount = 1/*NUM_SCISSORS*/;
		dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;
		vp.pScissors = NULL;
		vp.pViewports = NULL;
#else
		// Temporary disabling dynamic viewport on Android because some of drivers doesn't
		// support the feature.
		//VkViewport viewports;
		//viewports.minDepth = 0.0f;
		//viewports.maxDepth = 1.0f;
		//viewports.x = 0;
		//viewports.y = 0;
		//viewports.width = info.width;
		//viewports.height = info.height;
		//VkRect2D scissor;
		//scissor.extent.width = info.width;
		//scissor.extent.height = info.height;
		//scissor.offset.x = 0;
		//scissor.offset.y = 0;
		vp.viewportCount = 1/*NUM_VIEWPORTS*/;
		vp.scissorCount = 1/*NUM_SCISSORS*/;
		vp.pScissors = NULL;//&scissor;
		vp.pViewports = NULL;// &viewports;
#endif

		VkPipelineMultisampleStateCreateInfo ms;
		ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		ms.pNext = NULL;
		ms.flags = 0;
		ms.pSampleMask = NULL;
		ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT/*NUM_SAMPLES*/;
		ms.sampleShadingEnable = VK_FALSE;
		ms.alphaToCoverageEnable = VK_FALSE;
		ms.alphaToOneEnable = VK_FALSE;
		ms.minSampleShading = 0.0;

		VkGraphicsPipelineCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.pNext = NULL;
		pipelineCreateInfo.layout = m_graphicPip._Layout;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = 0;
		pipelineCreateInfo.flags = 0;
		pipelineCreateInfo.pVertexInputState = &pVertexDec->m_inputState;
		pipelineCreateInfo.pInputAssemblyState = &ia;
		pipelineCreateInfo.pRasterizationState = &pVulkanRS->rs;
		pipelineCreateInfo.pColorBlendState = &pVulkanBS->m_cb;
		pipelineCreateInfo.pTessellationState = NULL;
		pipelineCreateInfo.pMultisampleState = &ms;
		pipelineCreateInfo.pDynamicState = &dynamicState;
		pipelineCreateInfo.pViewportState = &vp;
		pipelineCreateInfo.pDepthStencilState = &pVulkanDS->ds;
		pipelineCreateInfo.pStages = this->m_shaderStages;
		pipelineCreateInfo.stageCount = ShaderType_Number -1; // vs + ps
 		VulkanRenderPass* pVulkanRenderPass = (VulkanRenderPass*)this->GetRenderPass();
		pipelineCreateInfo.renderPass = pVulkanRenderPass->m_impl;
		pipelineCreateInfo.subpass = 0;

		VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->logicalDevice, m_graphicPip._Cache, 1, &pipelineCreateInfo, NULL, &m_graphicPip._Pipeline));
	}

	void VulkanShaderProgram::CreateComputePipeline()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		VkPipelineCacheCreateInfo pipelineCache;
		pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		pipelineCache.pNext = NULL;
		pipelineCache.initialDataSize = 0;
		pipelineCache.pInitialData = NULL;
		pipelineCache.flags = 0;
		VK_CHECK_RESULT(vkCreatePipelineCache(device->logicalDevice, &pipelineCache, NULL, &m_computePip._Cache));

		// Create pipeline		
		VkComputePipelineCreateInfo computePipelineCreateInfo = vks::initializers::computePipelineCreateInfo(m_computePip._Layout, 0);
		computePipelineCreateInfo.stage = this->m_shaderStages[CS];
		VK_CHECK_RESULT(vkCreateComputePipelines(device->logicalDevice, m_computePip._Cache, 1, &computePipelineCreateInfo, nullptr, &m_computePip._Pipeline));
	}


	void VulkanShaderProgram::ParseShaderUniform(ShaderType eType,const vector<uint32_t>& vtx_spv)
	{
		spirv_cross::CompilerHLSL hlsl(vtx_spv.data(),vtx_spv.size());

		if (0)
		{
			// Set some options.
			spirv_cross::CompilerHLSL::Options options;
			//options.vulkan_semantics = true;
			hlsl.set_options(options);

			// Compile to GLSL, ready to give to GL driver.
			std::string source = hlsl.compile();

			LogInfo("%s", source.c_str());
		}

		spirv_cross::ShaderResources resources = hlsl.get_shader_resources();
		for (auto &resource : resources.uniform_buffers)
		{
			const spirv_cross::SPIRType& spType = hlsl.get_type(resource.type_id);
			size_t size_ = hlsl.get_declared_struct_size(spType);
			unsigned binding = hlsl.get_decoration(resource.id, spv::DecorationBinding);
			RefPtr<VulkanConstantBuffer> pConstantBuffer = new VulkanConstantBuffer();
			pConstantBuffer->SetName(resource.name.c_str());
			pConstantBuffer->SetBound(binding);
			pConstantBuffer->SetSize(size_);
			this->AddConstBuffer(eType, pConstantBuffer.get());
			for (uint32_t i = 0; i < spType.member_types.size(); ++i)
			{
				std::string str = hlsl.get_member_name(spType.self, i);
				size_t offset = hlsl.type_struct_member_offset(spType, i);
				size_t size = hlsl.get_declared_struct_member_size(spType, i);

				Uniform* pUniform = pConstantBuffer->AddUniform(str.c_str());
				pUniform->SetIndex(i);
				pUniform->SetOffset(offset);
				pUniform->SetSize(size);
			}
		}

		for (auto &resource : resources.separate_images)
		{
			unsigned binding = hlsl.get_decoration(resource.id, spv::DecorationBinding);

			RefPtr<Uniform> pUniform = CreateUniform(resource.name.c_str());
			pUniform->SetIndex(binding - m_texshiftBinding[eType]);

			this->AddSampler(eType, pUniform.get());
		}
	}

	void VulkanShaderProgram::RT_StreamComplete()
	{
		ASSERT(GetResState() == ResLoaded);

		Destory();

		const ShaderCreateInfo& info = this->GetShaderCreateInfo();

		CreateShaderMode(info.m_strVSFile, VS);

		CreateShaderMode(info.m_strPSFile, PS);

		CreatePipelineLayout();

		CreatePipelineCache();

		CreateDescriptorPool();

		CreateGraphicsPipeline();

		if (!info.m_strCSFile.empty())
		{
			CreateShaderMode(info.m_strCSFile, CS);
		
			CreateComputePipeline();
		}

		SetResState(ResInited);
	}

}

