#include "VulkanComputeShader.h"
#include "VulkanConstantBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderState.h"
#include "VulkanShaderUtil.h"


namespace ma
{

	VulkanComputeShader::VulkanComputeShader()
	{
	}

	VulkanComputeShader::~VulkanComputeShader()
	{
		Destory();
	}

	void VulkanComputeShader::Destory()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		if (m_pipeline)
		{
			vkDestroyPipeline(device->logicalDevice, m_pipeline, NULL);
		}

		if (m_cache)
		{
			vkDestroyPipelineCache(device->logicalDevice, m_cache, NULL);
		}

		if (m_desc_pool)
		{
			vkDestroyDescriptorPool(device->logicalDevice, m_desc_pool, NULL);
		}
	}

	void VulkanComputeShader::CreateDescriptorPool()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		// init_descriptor_pool
		VkDescriptorPoolSize type_count[3];
		type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		type_count[0].descriptorCount = MAX_SHADER_PARAMETER_GROUPS * 2 * 1000;

		type_count[1].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		type_count[1].descriptorCount = 16 * 1000;

		type_count[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		type_count[2].descriptorCount = 16 * 1000;

		VkDescriptorPoolCreateInfo descriptor_pool = {};
		descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptor_pool.pNext = NULL;
		descriptor_pool.maxSets = (MAX_SHADER_PARAMETER_GROUPS * 2 + 32) * 1000;
		descriptor_pool.poolSizeCount = 4;
		descriptor_pool.pPoolSizes = type_count;

		VK_CHECK_RESULT(vkCreateDescriptorPool(device->logicalDevice, &descriptor_pool, NULL, &m_desc_pool));

		ASSERT(m_desc_pool != VK_NULL_HANDLE);
	}

	void VulkanComputeShader::CreateComputePipeline(const VkPipelineShaderStageCreateInfo& stage)
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		{
			std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;

			// u
			for (uint32_t icb = 0; icb < MAX_TEXTURE_UNITS; ++icb)
			{
				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
					VK_SHADER_STAGE_COMPUTE_BIT,
					VulkanShaderUtil::uavshiftBinding(CS) + icb));
			}

			// t
			for (uint32_t icb = 0; icb < MAX_TEXTURE_UNITS; ++icb)
			{
				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
					VK_SHADER_STAGE_COMPUTE_BIT,
					VulkanShaderUtil::texshiftBinding(CS) + icb));
			}

			for (uint32_t icb = 0; icb < MAX_SHADER_PARAMETER_GROUPS; ++icb)
			{
				setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					VK_SHADER_STAGE_COMPUTE_BIT,
					VulkanShaderUtil::cbshiftBinding(CS) + icb));
			}

			VkDescriptorSetLayoutCreateInfo descriptorLayout;
			descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
				setLayoutBindings.data(),
				static_cast<uint32_t>(setLayoutBindings.size()));
			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorLayout, nullptr, &m_desc_layout));
		}

		/* Now use the descriptor layout to create a pipeline layout */
		VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
		pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pPipelineLayoutCreateInfo.pNext = NULL;
		pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;// pPushConstantRanges.size();
		pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;// pPushConstantRanges.data();
		pPipelineLayoutCreateInfo.setLayoutCount = 1;
		pPipelineLayoutCreateInfo.pSetLayouts = &m_desc_layout;

		VK_CHECK_RESULT(vkCreatePipelineLayout(device->logicalDevice, &pPipelineLayoutCreateInfo, NULL, &m_layout));

		VkPipelineCacheCreateInfo pipelineCache;
		pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		pipelineCache.pNext = NULL;
		pipelineCache.initialDataSize = 0;
		pipelineCache.pInitialData = NULL;
		pipelineCache.flags = 0;
		VK_CHECK_RESULT(vkCreatePipelineCache(device->logicalDevice, &pipelineCache, NULL, &m_cache));

		// Create pipeline		
		VkComputePipelineCreateInfo computePipelineCreateInfo = vks::initializers::computePipelineCreateInfo(m_layout, 0);
		computePipelineCreateInfo.stage = stage;
		VK_CHECK_RESULT(vkCreateComputePipelines(device->logicalDevice, m_cache, 1, &computePipelineCreateInfo, nullptr, &m_pipeline));
	}

	void VulkanComputeShader::RT_StreamComplete()
	{
		ASSERT(GetResState() == ResLoadIng);

		Destory();

		CreateDescriptorPool();

		VkPipelineShaderStageCreateInfo stage = CreateShaderMode(m_strFile, m_strMacro, CS);
		
		CreateComputePipeline(stage);
	
		SetResState(ResReady);
	}

}

