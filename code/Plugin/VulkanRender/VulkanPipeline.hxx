#include "VulkanPipeline.h"
#include "VulkanConstantBuffer.h"
#include "VulkanRenderState.h"

namespace ma
{

	VulkanPipeline::VulkanPipeline()
	{

	}

	VulkanPipeline::~VulkanPipeline()
	{

	}

	void VulkanPipeline::RT_StreamComplete(VulkanTechnique* pTech)
	{
		vks::VulkanDevice* device = GetVulkanDevice();
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		VulkanShaderProgram* pShader = (VulkanShaderProgram*)pTech->GetShaderProgram();

		VulkanVertexDeclaration* pVertexDec = (VulkanVertexDeclaration*)(pShader->GetVertexDeclaration());
		ASSERT(pVertexDec);
		if (pVertexDec->m_attributeDescriptions.empty())
		{
			pVertexDec->RT_StreamComplete();
		}

		VkResult res;

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
			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout[VS]));
		}

		{
			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
			for (UINT i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
			{
				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					i));
			}

			VkDescriptorSetLayoutCreateInfo descriptorLayout;
			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
				setLayoutBindings.data(),
				static_cast<uint32_t>(setLayoutBindings.size()));

			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout[PS]));
		}

		{
			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
			for (UINT i = 0; i < MAX_TEXTURE_UNITS/*pShader->GetSamplerCount()*/; ++i)
			{
				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_SAMPLER,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					i));
			}

			VkDescriptorSetLayoutCreateInfo descriptorLayout;
			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
				setLayoutBindings.data(),
				static_cast<uint32_t>(setLayoutBindings.size()));

			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout_sampler[0]));
		}

		{
			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
			for (UINT i = 0; i < MAX_TEXTURE_UNITS/*pShader->GetSamplerCount()*/; ++i)
			{
				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					i));
			}

			VkDescriptorSetLayoutCreateInfo descriptorLayout;
			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
				setLayoutBindings.data(),
				static_cast<uint32_t>(setLayoutBindings.size()));

			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout_sampler[1]));
		}

		VkDescriptorSetLayout SetLayouts[4];
		SetLayouts[0] = m_desc_layout[0];
		SetLayouts[1] = m_desc_layout[1];
		SetLayouts[2] = m_desc_layout_sampler[0];
		SetLayouts[3] = m_desc_layout_sampler[1];

		/* Now use the descriptor layout to create a pipeline layout */
		VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
		pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pPipelineLayoutCreateInfo.pNext = NULL;
		pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;// pPushConstantRanges.size();
		pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;// pPushConstantRanges.data();
		pPipelineLayoutCreateInfo.setLayoutCount = 4;
		pPipelineLayoutCreateInfo.pSetLayouts = SetLayouts;

		res = vkCreatePipelineLayout(device->logicalDevice, &pPipelineLayoutCreateInfo, NULL, &m_pipelineLayout);
		assert(res == VK_SUCCESS);

		VkPipelineCacheCreateInfo pipelineCache;
		pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		pipelineCache.pNext = NULL;
		pipelineCache.initialDataSize = 0;
		pipelineCache.pInitialData = NULL;
		pipelineCache.flags = 0;
		res = vkCreatePipelineCache(device->logicalDevice, &pipelineCache, NULL, &m_pipelineCache);
		assert(res == VK_SUCCESS);

		
		// init_descriptor_pool
		//VkResult res;
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

		res = vkCreateDescriptorPool(device->logicalDevice, &descriptor_pool, NULL, &m_desc_pool);
		assert(res == VK_SUCCESS);


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

		const VulkanRasterizerStateObject* rs = (const VulkanRasterizerStateObject*)pTech->GetRasterizerState();

		const VulkanBlendStateObject* bs = (const VulkanBlendStateObject*)pTech->GetBlendState();

		const VulkanDepthStencilStateObject* ds = (const VulkanDepthStencilStateObject*)pTech->GetDepthStencilState();

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
		pipelineCreateInfo.pRasterizationState = &rs->rs;
		pipelineCreateInfo.pColorBlendState = &bs->cb;
		pipelineCreateInfo.pTessellationState = NULL;
		pipelineCreateInfo.pMultisampleState = &ms;
		pipelineCreateInfo.pDynamicState = &dynamicState;
		pipelineCreateInfo.pViewportState = &vp;
		pipelineCreateInfo.pDepthStencilState =&ds->ds;
		pipelineCreateInfo.pStages = pShader->m_shaderStages;
		pipelineCreateInfo.stageCount = 2;
		VulkanRenderPass* pVulkanRenderPass = (VulkanRenderPass*)pTech->GetRenderPass();
		pipelineCreateInfo.renderPass = pVulkanRenderPass->m_impl;
		pipelineCreateInfo.subpass = 0;

		res = vkCreateGraphicsPipelines(device->logicalDevice, m_pipelineCache, 1, &pipelineCreateInfo, NULL, &m_pipeline);
		assert(res == VK_SUCCESS);

		return;
	}


 	struct InfoKey
	{
		ShaderProgram* m_pShader;

		const BlendState* m_pBlendState;

		const DepthStencilState* m_pDSState;

		const RasterizerState* m_pRSState;

		bool operator < (const InfoKey& other) const
		{
			if (m_pShader != other.m_pShader)
				return m_pShader < other.m_pShader;

// 			if (m_pBlendState != other.m_pBlendState)
// 				return m_pBlendState < other.m_pBlendState;
// 
// 			if (m_pDSState != other.m_pDSState)
// 				return m_pDSState < other.m_pDSState;
// 
// 			if (m_pRSState != other.m_pRSState)
// 				return m_pRSState < other.m_pRSState;

			return false;
		}
	};

	std::map< InfoKey, RefPtr<VulkanPipeline> > g_mapPiplinePool;

	RefPtr<VulkanPipeline> CreateVulkanPipeline(VulkanTechnique* pTech)
	{
		InfoKey key;
		key.m_pShader = pTech->GetShaderProgram();
		key.m_pBlendState = pTech->GetBlendState();
		key.m_pDSState = pTech->GetDepthStencilState();
		auto it = g_mapPiplinePool.find(key);
		if (it != g_mapPiplinePool.end())
		{
			return it->second;
		}
		else
		{
			RefPtr<VulkanPipeline> pPP = new VulkanPipeline();
			pPP->RT_StreamComplete(pTech);

			g_mapPiplinePool.insert(std::make_pair(key, pPP));

			return pPP;
		}
	}
}


