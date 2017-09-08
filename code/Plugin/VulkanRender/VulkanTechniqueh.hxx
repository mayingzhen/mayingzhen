#include "VulkanShaderProgram.h"
#include "VulkanConstantBuffer.h"
#include "..\glslang\glslang\Include\ResourceLimits.h"
#include "..\glslang\glslang\Public\ShaderLang.h"
#include "..\glslang\SPIRV\GlslangToSpv.h"

#include "..\SPIRV-Cross\spirv_glsl.hpp"
#include "..\..\..\SPIRV-Cross\spirv_hlsl.hpp"
#include "..\..\..\SPIRV-Cross\spirv_msl.hpp"
#include "VulkanRenderState.h"

namespace ma
{

	VulkanTechnique::VulkanTechnique()
	{

	}

	VulkanTechnique::~VulkanTechnique()
	{

	}

	void VulkanTechnique::CommitChanges()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		VulkanShaderProgram* pShader = (VulkanShaderProgram*)this->GetShaderProgram();

		VkCommandBuffer cmdBuffer = pRender->m_drawCmdBuffers[pRender->m_currentBuffer];

		if (m_bSamplerDirty)
		{
			{
				VkDescriptorSetAllocateInfo alloc_info[1];
				alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				alloc_info[0].pNext = NULL;
				alloc_info[0].descriptorPool = m_desc_pool;
				alloc_info[0].descriptorSetCount = 1;
				alloc_info[0].pSetLayouts = &m_desc_layout_sampler[0];

				VkResult res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_descriptorSets_sampler[0]);
				assert(res == VK_SUCCESS);

				std::vector<VkWriteDescriptorSet> vec_write;
				for (uint32 i = 0; i < pShader->m_vecPSSamplers.size(); ++i)
				{
					uint32 nIndex = pShader->m_vecPSSamplers[i]->m_index;
					VulkanSamplerStateObject* pSampler = (VulkanSamplerStateObject*)pRender->m_arrSampler[nIndex];
					if (pSampler == NULL)
						continue;

					pSampler->RT_StreamComplete();

					VkWriteDescriptorSet write = {};
					write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					write.pNext = NULL;
					write.dstSet = m_descriptorSets_sampler[0];
					write.descriptorCount = 1;
					write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
					write.pImageInfo = &pSampler->m_descriptor;
					write.dstArrayElement = 0;
					write.dstBinding = pShader->m_vecPSSamplers[i]->m_index;
					vec_write.push_back(write);

				}

				vkUpdateDescriptorSets(device->logicalDevice, vec_write.size(), vec_write.data(), 0, NULL);
			}

			{
				VkDescriptorSetAllocateInfo alloc_info[1];
				alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				alloc_info[0].pNext = NULL;
				alloc_info[0].descriptorPool = m_desc_pool;
				alloc_info[0].descriptorSetCount = 1;
				alloc_info[0].pSetLayouts = &m_desc_layout_sampler[1];

				VkResult res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_descriptorSets_sampler[1]);
				assert(res == VK_SUCCESS);

				std::vector<VkWriteDescriptorSet> vec_write;
				for (uint32 i = 0; i < pShader->m_vecPSSamplers.size(); ++i)
				{
					uint32 nIndex = pShader->m_vecPSSamplers[i]->m_index;
					VulkanSamplerStateObject* pSampler = (VulkanSamplerStateObject*)pRender->m_arrSampler[nIndex];
					if (pSampler == NULL)
						continue;

					pSampler->RT_StreamComplete();

					VkWriteDescriptorSet write = {};
					write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					write.pNext = NULL;
					write.dstSet = m_descriptorSets_sampler[1];
					write.descriptorCount = 1;
					write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					write.pImageInfo = &pSampler->m_descriptor;
					write.dstArrayElement = 0;
					write.dstBinding = pShader->m_vecPSSamplers[i]->m_index;
					vec_write.push_back(write);

				}

				vkUpdateDescriptorSets(device->logicalDevice, vec_write.size(), vec_write.data(), 0, NULL);
			}


			m_bSamplerDirty = false;
		}

		VkDescriptorSet descriptorSets[4];
		descriptorSets[0] = m_descriptorSets[0];
		descriptorSets[1] = m_descriptorSets[1];
		descriptorSets[2] = m_descriptorSets_sampler[0];
		descriptorSets[3] = m_descriptorSets_sampler[1];

		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
		vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0,
			4, descriptorSets, 0, NULL);

	}

	void VulkanTechnique::RT_StreamComplete()
	{
		vks::VulkanDevice* device = GetVulkanDevice();
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		VulkanShaderProgram* pShader = (VulkanShaderProgram*)this->GetShaderProgram();

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
			for (UINT i = 0; i < pShader->m_vecPSSamplers.size(); ++i)
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
			for (UINT i = 0; i < pShader->m_vecPSSamplers.size(); ++i)
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
		type_count[0].descriptorCount = MAX_SHADER_PARAMETER_GROUPS * 2;

		type_count[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
		type_count[1].descriptorCount = 16;

		type_count[2].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		type_count[2].descriptorCount = 16;

		VkDescriptorPoolCreateInfo descriptor_pool = {};
		descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptor_pool.pNext = NULL;
		descriptor_pool.maxSets = MAX_SHADER_PARAMETER_GROUPS * 2 + 32;
		descriptor_pool.poolSizeCount = 3;
		descriptor_pool.pPoolSizes = type_count;

		res = vkCreateDescriptorPool(device->logicalDevice, &descriptor_pool, NULL, &m_desc_pool);
		assert(res == VK_SUCCESS);

		//init_descriptor_set
		{
			VkDescriptorSetAllocateInfo alloc_info[1];
			alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info[0].pNext = NULL;
			alloc_info[0].descriptorPool = m_desc_pool;
			alloc_info[0].descriptorSetCount = 1;
			alloc_info[0].pSetLayouts = &m_desc_layout[VS];

			res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_descriptorSets[VS]);
			assert(res == VK_SUCCESS);

			std::vector<VkWriteDescriptorSet> vec_write;
			for (uint32 i = 0; i < pShader->m_vecVSConstantBuffers.size(); ++i)
			{
				VkWriteDescriptorSet write = {};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = NULL;
				write.dstSet = m_descriptorSets[VS];
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				write.pBufferInfo = &pShader->m_vecVSConstantBuffers[i]->m_descriptor;
				write.dstArrayElement = 0;
				write.dstBinding = pShader->m_vecVSConstantBuffers[i]->m_nBound;
				vec_write.push_back(write);
			}

			vkUpdateDescriptorSets(device->logicalDevice, vec_write.size(), vec_write.data(), 0, NULL);
		}

		// PS
		{
			VkDescriptorSetAllocateInfo alloc_info[1];
			alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info[0].pNext = NULL;
			alloc_info[0].descriptorPool = m_desc_pool;
			alloc_info[0].descriptorSetCount = 1;
			alloc_info[0].pSetLayouts = &m_desc_layout[PS];

			res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_descriptorSets[PS]);
			assert(res == VK_SUCCESS);

			std::vector<VkWriteDescriptorSet> vec_write;
			for (uint32 i = 0; i < pShader->m_vecPSConstantBuffers.size(); ++i)
			{
				VkWriteDescriptorSet write = {};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = NULL;
				write.dstSet = m_descriptorSets[PS];
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				write.pBufferInfo = &pShader->m_vecPSConstantBuffers[i]->m_descriptor;
				write.dstArrayElement = 0;
				write.dstBinding = pShader->m_vecPSConstantBuffers[i]->m_nBound;
				vec_write.push_back(write);
			}

			vkUpdateDescriptorSets(device->logicalDevice, vec_write.size(), vec_write.data(), 0, NULL);
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

		const VulkanRasterizerStateObject* rs = (const VulkanRasterizerStateObject*)this->GetRasterizerState();

		const VulkanBlendStateObject* bs = (const VulkanBlendStateObject*)this->GetBlendState();

		const VulkanDepthStencilStateObject* ds = (const VulkanDepthStencilStateObject*)this->GetDepthStencilState();

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
		pipelineCreateInfo.renderPass = pRender->m_renderPass;
		pipelineCreateInfo.subpass = 0;

		res = vkCreateGraphicsPipelines(device->logicalDevice, m_pipelineCache, 1, &pipelineCreateInfo, NULL, &m_pipeline);
		assert(res == VK_SUCCESS);

		return;
	}

	void VulkanTechnique::SetTexture(Uniform* uniform, const SamplerState* sampler)
	{

	}


}

