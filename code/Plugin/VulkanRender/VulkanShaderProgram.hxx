#include "VulkanShaderProgram.h"
#include "VulkanConstantBuffer.h"
#include "..\glslang\glslang\Include\ResourceLimits.h"
#include "..\glslang\glslang\Public\ShaderLang.h"
#include "..\glslang\SPIRV\GlslangToSpv.h"

#include "..\SPIRV-Cross\spirv_glsl.hpp"
#include "..\..\..\SPIRV-Cross\spirv_hlsl.hpp"

namespace ma
{

	//vector<ConstantBuffer*> g_vecDirtyConstantBuffers;
	//IVulkanBuffer* g_vecVulkanConstantBuffers[2][MAX_SHADER_PARAMETER_GROUPS];

	VulkanShaderProgram::VulkanShaderProgram()
	{
		//m_pVertexShader = NULL;
		//m_pPiexelShader = NULL;
	}

	VulkanShaderProgram::~VulkanShaderProgram()
	{
		Destory();
	}

	void VulkanShaderProgram::Destory()
	{
// 		SAFE_RELEASE(m_pVertexShader);
// 		SAFE_RELEASE(m_pPiexelShader);
// 
// 		for (uint32 i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
// 		{
// 			m_vecVSConstantBuffers[i] = NULL;
// 			m_vecPSConstantBuffers[i] = NULL;
// 		}
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

	void VulkanShaderProgram::CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize)
	{
		Destory();

		VkShaderModuleCreateInfo moduleCreateInfo;

		vks::VulkanDevice* device = GetVulkanDevice();

		//VS
		{
			std::vector<unsigned int> vtx_spv;
			m_shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			m_shaderStages[0].pNext = NULL;
			m_shaderStages[0].pSpecializationInfo = NULL;
			m_shaderStages[0].flags = 0;
			m_shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
			m_shaderStages[0].pName = "main";

			EShLanguage stage = FindLanguage(VK_SHADER_STAGE_VERTEX_BIT);
			glslang::TShader shader(stage);
			glslang::TProgram program;
			const char *shaderStrings[1];
			TBuiltInResource Resources;
			init_resources(Resources);

			// Enable SPIR-V and Vulkan rules when parsing GLSL
			EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgReadHlsl);

			//shaderStrings[0] = "#version 100\n";
			shaderStrings[0] = vshSource;
			shader.setStrings(shaderStrings, 1);
			shader.setEntryPoint("main");

			if (!shader.parse(&Resources, 100, false, messages)) {
				puts(shader.getInfoLog());
				puts(shader.getInfoDebugLog());
				return;  // something didn't work
			}

			program.addShader(&shader);

			//
			// Program-level processing...
			//

			if (!program.link(messages)) {
				puts(shader.getInfoLog());
				puts(shader.getInfoDebugLog());
				fflush(stdout);
				return;
			}

			glslang::GlslangToSpv(*program.getIntermediate(stage), vtx_spv);

			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.pNext = NULL;
			moduleCreateInfo.flags = 0;
			moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
			moduleCreateInfo.pCode = vtx_spv.data();
			VkResult res = vkCreateShaderModule(device->logicalDevice, &moduleCreateInfo, NULL, &m_shaderStages[0].module);
			assert(res == VK_SUCCESS);

			spirv_cross::CompilerGLSL glsl(std::move(vtx_spv));
			spirv_cross::ShaderResources resources = glsl.get_shader_resources();
			for (auto &resource : resources.uniform_buffers)
			{
				const spirv_cross::SPIRType& spType = glsl.get_type(resource.type_id);
				size_t size_ = glsl.get_declared_struct_size(spType);
				unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
				RefPtr<ConstantBuffer> pConstantBuffer = CreateConstantBuffer(VS, binding, size_);
				m_vecVSConstantBuffers.push_back(pConstantBuffer);
				for (UINT i = 0; i < spType.member_types.size(); ++i)
				{
					std::string str = glsl.get_member_name(spType.self, i);
					size_t offset = glsl.type_struct_member_offset(spType, i);
					size_t size = glsl.get_declared_struct_member_size(spType, i);

					Uniform* pUniform = this->AddUniform(str.c_str());
					pUniform->m_index = i;
					pUniform->m_vshShder = true;

					pUniform->m_nCBOffset = offset;
					pUniform->m_nCBSize = size;
					pUniform->m_pD3D11CBPtr = pConstantBuffer.get();
				}
			}


			{
				// Set some options.
				spirv_cross::CompilerGLSL::Options options;
				//options.version = 310;
				//options.es = false;
				options.vulkan_semantics = true;
				glsl.set_options(options);

				// Compile to GLSL, ready to give to GL driver.
				std::string source = glsl.compile();

				int i = 0;
			}

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

			EShLanguage stage = FindLanguage(VK_SHADER_STAGE_FRAGMENT_BIT);
			glslang::TShader shader(stage);
			glslang::TProgram program;
			const char *shaderStrings[1];
			TBuiltInResource Resources;
			init_resources(Resources);

			// Enable SPIR-V and Vulkan rules when parsing GLSL
			EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgReadHlsl);

			//shaderStrings[0] = "#version 100\n";
			shaderStrings[0] = fshSource;
			shader.setStrings(shaderStrings, 1);
			shader.setEntryPoint("main");

			if (!shader.parse(&Resources, 100, false, messages)) {
				puts(shader.getInfoLog());
				puts(shader.getInfoDebugLog());
				return;  // something didn't work
			}

			program.addShader(&shader);

			//
			// Program-level processing...
			//

			if (!program.link(messages)) {
				puts(shader.getInfoLog());
				puts(shader.getInfoDebugLog());
				fflush(stdout);
				return;
			}

			glslang::GlslangToSpv(*program.getIntermediate(stage), fsh_spv);

			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.pNext = NULL;
			moduleCreateInfo.flags = 0;
			moduleCreateInfo.codeSize = fsh_spv.size() * sizeof(unsigned int);
			moduleCreateInfo.pCode = fsh_spv.data();
			VkResult res = vkCreateShaderModule(device->logicalDevice, &moduleCreateInfo, NULL, &m_shaderStages[1].module);
			assert(res == VK_SUCCESS);

			spirv_cross::CompilerGLSL glsl(std::move(fsh_spv));
			spirv_cross::ShaderResources resources = glsl.get_shader_resources();
			for (auto &resource : resources.uniform_buffers)
			{
				const spirv_cross::SPIRType& spType = glsl.get_type(resource.type_id);
				size_t size_ = glsl.get_declared_struct_size(spType);
				unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
				RefPtr<ConstantBuffer> pConstantBuffer = CreateConstantBuffer(PS, binding, size_);
				m_vecPSConstantBuffers.push_back(pConstantBuffer);
				for (UINT i = 0; i < spType.member_types.size(); ++i)
				{
					std::string str = glsl.get_member_name(spType.self,i);
					size_t offset = glsl.type_struct_member_offset(spType, i);
					size_t size = glsl.get_declared_struct_member_size(spType, i);

					Uniform* pUniform = this->AddUniform( str.c_str() );
					pUniform->m_index = i;
					pUniform->m_vshShder = false;

					pUniform->m_nCBOffset = offset;
					pUniform->m_nCBSize = size;
					pUniform->m_pD3D11CBPtr = pConstantBuffer.get();
				}
			}

			{
				// Set some options.
				spirv_cross::CompilerGLSL::Options options;
				//options.version = 310;
				//options.es = false;
				options.vulkan_semantics = true;
				glsl.set_options(options);

				// Compile to GLSL, ready to give to GL driver.
				std::string source = glsl.compile();

				int i = 0;
			}
		}

		VulkanVertexDeclaration* pVertexDec = (VulkanVertexDeclaration*)(this->GetVertexDeclaration());
		ASSERT(pVertexDec);
		if (pVertexDec->m_attributeDescriptions.empty())
		{
			pVertexDec->RT_StreamComplete();
		}

		VkResult res;
		
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();
		vks::VulkanDevice* pDevice = GetVulkanDevice();

// 		std::vector<VkDescriptorPoolSize> poolSizes;
// 		size_t ubSize = m_vecPSConstantBuffers.size() + m_vecPSConstantBuffers.size();
// 		poolSizes.push_back(vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubSize));
// 
// 		VkDescriptorPool descriptorPool;
// 
// 		VkDescriptorPoolCreateInfo descriptorPoolInfo =
// 			vks::initializers::descriptorPoolCreateInfo(
// 				static_cast<uint32_t>(poolSizes.size()),
// 				poolSizes.data(),
// 				static_cast<uint32_t>(ubSize) + 1);
// 
// 		VK_CHECK_RESULT(vkCreateDescriptorPool(pDevice->logicalDevice, &descriptorPoolInfo, nullptr, &descriptorPool));


		//std::vector<VkDescriptorSetLayout> vec_layout;
		//for (UINT i = 0; i < m_vecVSConstantBuffers.size(); ++i)
		{
			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
			for (UINT i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
			{
				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					VK_SHADER_STAGE_VERTEX_BIT,
					i));
			}

			VkDescriptorSetLayoutCreateInfo descriptorLayout;
			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
				setLayoutBindings.data(),
				static_cast<uint32_t>(setLayoutBindings.size()));

			//VkDescriptorSetLayout desc_layout;
			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(pDevice->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout));

			//vec_layout.push_back(desc_layout);
		}

// 		for (UINT i = 0; i < m_vecPSConstantBuffers.size(); ++i)
// 		{
// 			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
// 			setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
// 				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
// 				VK_SHADER_STAGE_FRAGMENT_BIT,
// 				0));
// 
// 			VkDescriptorSetLayoutCreateInfo descriptorLayout;
// 			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
// 				setLayoutBindings.data(),
// 				static_cast<uint32_t>(setLayoutBindings.size()));
// 
// 			VkDescriptorSetLayout desc_layout;
// 			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(pDevice->logicalDevice, &descriptorLayout, nullptr, &desc_layout));
// 
// 			vec_layout.push_back(desc_layout);
// 		}

// 		for (UINT i = 0; i < m_vecPSConstantBuffers.size(); ++i)
// 		{
// 			VkPushConstantRange pushConstantRange = vks::initializers::pushConstantRange(
// 				VK_SHADER_STAGE_FRAGMENT_BIT,
// 				m_vecPSConstantBuffers[i]->GetSize(),
// 				0);
// 			pPushConstantRanges.push_back(pushConstantRange);
// 		}


		//desc_layout.resize(1/*NUM_DESCRIPTOR_SETS*/);
		//res = vkCreateDescriptorSetLayout(pDevice->logicalDevice, &descriptor_layout, NULL, desc_layout.data());
		//assert(res == VK_SUCCESS);

//  		std::vector<VkPushConstantRange> pPushConstantRanges;
// 		for (UINT i = 0; i < m_vecVSConstantBuffers.size(); ++i)
// 		{
// 			VkPushConstantRange pushConstantRange = vks::initializers::pushConstantRange(
// 				VK_SHADER_STAGE_VERTEX_BIT,
// 				m_vecVSConstantBuffers[i]->GetSize(),
// 				0);
// 			pPushConstantRanges.push_back(pushConstantRange);
// 		}
// 
// 		for (UINT i = 0; i < m_vecPSConstantBuffers.size(); ++i)
// 		{
// 			VkPushConstantRange pushConstantRange = vks::initializers::pushConstantRange(
// 				VK_SHADER_STAGE_FRAGMENT_BIT,
// 				m_vecPSConstantBuffers[i]->GetSize(),
// 				0);
// 			pPushConstantRanges.push_back(pushConstantRange);
// 		}

		/* Now use the descriptor layout to create a pipeline layout */
		VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
		pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pPipelineLayoutCreateInfo.pNext = NULL;
		pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;// pPushConstantRanges.size();
		pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;// pPushConstantRanges.data();
		pPipelineLayoutCreateInfo.setLayoutCount = 1;
		pPipelineLayoutCreateInfo.pSetLayouts = &m_desc_layout;

		res = vkCreatePipelineLayout(pDevice->logicalDevice, &pPipelineLayoutCreateInfo, NULL, &m_pipelineLayout);
		assert(res == VK_SUCCESS);

		VkPipelineCacheCreateInfo pipelineCache;
		pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		pipelineCache.pNext = NULL;
		pipelineCache.initialDataSize = 0;
		pipelineCache.pInitialData = NULL;
		pipelineCache.flags = 0;
		res = vkCreatePipelineCache(pDevice->logicalDevice, &pipelineCache, NULL, &m_pipelineCache);
		assert(res == VK_SUCCESS);

		
		// init_descriptor_pool
		//VkResult res;
		VkDescriptorPoolSize type_count[1];
		type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		type_count[0].descriptorCount = MAX_SHADER_PARAMETER_GROUPS * 2;

		VkDescriptorPoolCreateInfo descriptor_pool = {};
		descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptor_pool.pNext = NULL;
		descriptor_pool.maxSets = MAX_SHADER_PARAMETER_GROUPS * 2;
		descriptor_pool.poolSizeCount = 1;
		descriptor_pool.pPoolSizes = type_count;

		res = vkCreateDescriptorPool(pDevice->logicalDevice, &descriptor_pool, NULL, &m_desc_pool);
		assert(res == VK_SUCCESS);

		//init_descriptor_set
		VkDescriptorSetAllocateInfo alloc_info[1];
		alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info[0].pNext = NULL;
		alloc_info[0].descriptorPool = m_desc_pool;
		alloc_info[0].descriptorSetCount = 1;
		alloc_info[0].pSetLayouts = &m_desc_layout;

		VkDescriptorSet desc_set;

		res = vkAllocateDescriptorSets(pDevice->logicalDevice, alloc_info, &desc_set);
		assert(res == VK_SUCCESS);

		m_descriptorSets.push_back(desc_set);

		std::vector<VkWriteDescriptorSet> writes;
		writes.resize(m_vecVSConstantBuffers.size());
		for (uint32 i = 0; i < m_vecVSConstantBuffers.size(); ++i)
		{
			writes[i] = {};
			writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[i].pNext = NULL;
			writes[i].dstSet = desc_set;
			writes[i].descriptorCount = 1;
			writes[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writes[i].pBufferInfo = &m_vecVSConstantBuffers[i]->m_descriptor;
			writes[i].dstArrayElement = 0;
			writes[i].dstBinding = m_vecVSConstantBuffers[i]->m_nBound;

		}

		vkUpdateDescriptorSets(pDevice->logicalDevice, writes.size(), writes.data(), 0, NULL);
		

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

		VkPipelineRasterizationStateCreateInfo rs;
		rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rs.pNext = NULL;
		rs.flags = 0;
		rs.polygonMode = VK_POLYGON_MODE_FILL;
		rs.cullMode = VK_CULL_MODE_BACK_BIT;
		rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rs.depthClampEnable = VK_FALSE;
		rs.rasterizerDiscardEnable = VK_FALSE;
		rs.depthBiasEnable = VK_FALSE;
		rs.depthBiasConstantFactor = 0;
		rs.depthBiasClamp = 0;
		rs.depthBiasSlopeFactor = 0;
		rs.lineWidth = 1.0f;

		VkPipelineColorBlendStateCreateInfo cb;
		cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		cb.flags = 0;
		cb.pNext = NULL;
		VkPipelineColorBlendAttachmentState att_state[1];
		att_state[0].colorWriteMask = 0xf;
		att_state[0].blendEnable = VK_FALSE;
		att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;
		att_state[0].colorBlendOp = VK_BLEND_OP_ADD;
		att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		cb.attachmentCount = 1;
		cb.pAttachments = att_state;
		cb.logicOpEnable = VK_FALSE;
		cb.logicOp = VK_LOGIC_OP_NO_OP;
		cb.blendConstants[0] = 1.0f;
		cb.blendConstants[1] = 1.0f;
		cb.blendConstants[2] = 1.0f;
		cb.blendConstants[3] = 1.0f;

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
		VkViewport viewports;
		viewports.minDepth = 0.0f;
		viewports.maxDepth = 1.0f;
		viewports.x = 0;
		viewports.y = 0;
		viewports.width = info.width;
		viewports.height = info.height;
		VkRect2D scissor;
		scissor.extent.width = info.width;
		scissor.extent.height = info.height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vp.viewportCount = NUM_VIEWPORTS;
		vp.scissorCount = NUM_SCISSORS;
		vp.pScissors = &scissor;
		vp.pViewports = &viewports;
#endif
		VkPipelineDepthStencilStateCreateInfo ds;
		ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		ds.pNext = NULL;
		ds.flags = 0;
		ds.depthTestEnable = true/*include_depth*/;
		ds.depthWriteEnable = true/*include_depth*/;
		ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		ds.depthBoundsTestEnable = VK_FALSE;
		ds.stencilTestEnable = VK_FALSE;
		ds.back.failOp = VK_STENCIL_OP_KEEP;
		ds.back.passOp = VK_STENCIL_OP_KEEP;
		ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
		ds.back.compareMask = 0;
		ds.back.reference = 0;
		ds.back.depthFailOp = VK_STENCIL_OP_KEEP;
		ds.back.writeMask = 0;
		ds.minDepthBounds = 0;
		ds.maxDepthBounds = 0;
		ds.stencilTestEnable = VK_FALSE;
		ds.front = ds.back;

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
		pipelineCreateInfo.layout = m_pipelineLayout;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = 0;
		pipelineCreateInfo.flags = 0;
		pipelineCreateInfo.pVertexInputState = &pVertexDec->m_inputState;
		pipelineCreateInfo.pInputAssemblyState = &ia;
		pipelineCreateInfo.pRasterizationState = &rs;
		pipelineCreateInfo.pColorBlendState = &cb;
		pipelineCreateInfo.pTessellationState = NULL;
		pipelineCreateInfo.pMultisampleState = &ms;
		pipelineCreateInfo.pDynamicState = &dynamicState;
		pipelineCreateInfo.pViewportState = &vp;
		pipelineCreateInfo.pDepthStencilState =&ds;
		pipelineCreateInfo.pStages = m_shaderStages;
		pipelineCreateInfo.stageCount = 2;
		pipelineCreateInfo.renderPass = pRender->m_renderPass;
		pipelineCreateInfo.subpass = 0;

		res = vkCreateGraphicsPipelines(pDevice->logicalDevice, m_pipelineCache, 1, &pipelineCreateInfo, NULL, &m_pipeline);
		assert(res == VK_SUCCESS);


		return;
	}


	void VulkanShaderProgram::ParseUniform()
	{

	}

	void VulkanShaderProgram::ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
		RefPtr<ConstantBuffer> ConstantBuffersPtr[])
	{
	}

	void VulkanShaderProgram::RT_SetShader()
	{
	}

	void VulkanShaderProgram::CommitChanges()
	{
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		VkCommandBuffer cmdBuffer = pRender->m_drawCmdBuffers[pRender->currentBuffer];

		// todo : per material pipelines
		// vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *mesh.material->pipeline);

		// We will be using multiple descriptor sets for rendering
		// In GLSL the selection is done via the set and binding keywords
		// VS: layout (set = 0, binding = 0) uniform UBO;
		// FS: layout (set = 1, binding = 0) uniform sampler2D samplerColorMap;

// 		std::array<VkDescriptorSet, 2> descriptorSets;
// 		// Set 0: Scene descriptor set containing global matrices
// 		descriptorSets[0] = descriptorSetScene;
// 		// Set 1: Per-Material descriptor set containing bound images
// 		descriptorSets[1] = meshes[i].material->descriptorSet;

		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
		vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0,
			m_descriptorSets.size(), m_descriptorSets.data(), 0, NULL);

		// Pass material properies via push constants
// 		vkCmdPushConstants(
// 			cmdBuffer,
// 			m_pipelineLayout,
// 			VK_SHADER_STAGE_FRAGMENT_BIT,
// 			0,
// 			sizeof(SceneMaterialProperites),
// 			&meshes[i].material->properties);


		for (UINT i = 0; i < m_vecVSConstantBuffers.size(); ++i)
		{
			m_vecVSConstantBuffers[i]->Apply();
		}

		for (UINT i = 0; i < m_vecPSConstantBuffers.size(); ++i)
		{
			m_vecPSConstantBuffers[i]->Apply();
		}
	}

}

