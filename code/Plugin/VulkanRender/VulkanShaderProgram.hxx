#include "VulkanShaderProgram.h"
#include "VulkanConstantBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanShaderUtil.h"

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
			m_uavshiftBinding[i] = m_samplershiftBinding[i] + MAX_TEXTURE_UNITS;
			offset = m_uavshiftBinding[i] + MAX_TEXTURE_UNITS;
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
					VK_SHADER_STAGE_GEOMETRY_BIT,
					m_cbshiftBinding[GS] + icb));

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
			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_graphicPip.m_desc_layout));
		}

		/* Now use the descriptor layout to create a pipeline layout */
		VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
		pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pPipelineLayoutCreateInfo.pNext = NULL;
		pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;// pPushConstantRanges.size();
		pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;// pPushConstantRanges.data();
		pPipelineLayoutCreateInfo.setLayoutCount = 1;
		pPipelineLayoutCreateInfo.pSetLayouts = &m_graphicPip.m_desc_layout;

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
		VkDescriptorPoolSize type_count[4];
		type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		type_count[0].descriptorCount = MAX_SHADER_PARAMETER_GROUPS * 2 * 1000;

		type_count[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
		type_count[1].descriptorCount = 16 * 1000;

		type_count[2].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		type_count[2].descriptorCount = 16 * 1000;

		type_count[3].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		type_count[3].descriptorCount = 16 * 1000;

		VkDescriptorPoolCreateInfo descriptor_pool = {};
		descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptor_pool.pNext = NULL;
		descriptor_pool.maxSets = (MAX_SHADER_PARAMETER_GROUPS * 2 + 32) * 1000;
		descriptor_pool.poolSizeCount = 4;
		descriptor_pool.pPoolSizes = type_count;

		VK_CHECK_RESULT(vkCreateDescriptorPool(device->logicalDevice, &descriptor_pool, NULL, &m_desc_pool));

		ASSERT(m_desc_pool != VK_NULL_HANDLE);
	}


	void VulkanShaderProgram::CreateGraphicsPipeline()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		const ShaderCreateInfo& info = this->GetShaderCreateInfo();

		VulkanBlendState bs;
		VulkanBlendState* pVulkanBS = (VulkanBlendState*)(info.m_pBlendState.get());
		if (pVulkanBS)
		{
			pVulkanBS->RT_StreamComplete();
		}
		else
		{
			pVulkanBS = &bs;
		}

		pVulkanBS->m_cb.attachmentCount = GetRenderPass()->m_arrColor.size();

		VulkanDepthStencilState ds;
		VulkanDepthStencilState* pVulkanDS = (VulkanDepthStencilState*)(info.m_pDSState.get());
		if (pVulkanDS)
		{
			pVulkanDS->RT_StreamComplete();
		}
		else
		{
			pVulkanDS = &ds;
		}

		VulkanRasterizerState rs;
		VulkanRasterizerState* pVulkanRS = (VulkanRasterizerState*)(info.m_pRSState.get());
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

		std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		if (pVulkanDS && pVulkanDS->m_bDepthBoundsTest)
		{
			dynamicStateEnables.push_back(VK_DYNAMIC_STATE_DEPTH_BOUNDS);
		}
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.pNext = NULL;
		dynamicState.pDynamicStates = dynamicStateEnables.data();
		dynamicState.dynamicStateCount = dynamicStateEnables.size();

		VkPipelineInputAssemblyStateCreateInfo ia;
		ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		ia.pNext = NULL;
		ia.flags = 0;
		ia.primitiveRestartEnable = VK_FALSE;
		ia.topology = VulkanMapping::GetPrimitiveType(info.m_ePrimitiveType);// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo vp = {};
		vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		vp.pNext = NULL;
		vp.flags = 0;
#ifndef __ANDROID__
		vp.viewportCount = 1/*NUM_VIEWPORTS*/;
		//dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
		vp.scissorCount = 1/*NUM_SCISSORS*/;
		//dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;
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
		pipelineCreateInfo.pStages = m_shaderStages.data();
		pipelineCreateInfo.stageCount = m_shaderStages.size();
 		VulkanRenderPass* pVulkanRenderPass = (VulkanRenderPass*)this->GetRenderPass();
		pipelineCreateInfo.renderPass = pVulkanRenderPass->m_impl;
		pipelineCreateInfo.subpass = 0;

		VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->logicalDevice, m_graphicPip._Cache, 1, &pipelineCreateInfo, NULL, &m_graphicPip._Pipeline));
	}

	void VulkanShaderProgram::CreateShaderMode(const std::string& shaderFile, const std::string shaderMacro, ShaderType type)
	{
		VkPipelineShaderStageCreateInfo shaderStage;

		UniformData uniform;

		VulkanShaderUtil::CreateShaderMode(shaderFile, shaderMacro, type, shaderStage, uniform);

		m_shaderStages.emplace_back(shaderStage);

	
	}

	void VulkanShaderProgram::RT_StreamComplete()
	{
		ASSERT(GetResState() == ResLoadIng);

		Destory();

		CreateDescriptorPool();

		const ShaderCreateInfo& info = this->GetShaderCreateInfo();
	
		CreateShaderMode(info.m_strVSFile, info.m_shaderMacro, VS);

		CreateShaderMode(info.m_strPSFile, info.m_shaderMacro, PS);

		if (!info.m_strGSFile.empty())
		{
			CreateShaderMode(info.m_strGSFile, info.m_shaderMacro, GS);
		}

		CreatePipelineLayout();

		CreatePipelineCache();

		CreateGraphicsPipeline();
	
		SetResState(ResReady);
	}

}

