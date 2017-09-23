#include "VulkanTechniqueh.h"
#include "VulkanConstantBuffer.h"
#include "VulkanPipeline.h"
#include "..\glslang\glslang\Include\ResourceLimits.h"
#include "..\glslang\glslang\Public\ShaderLang.h"
#include "..\glslang\SPIRV\GlslangToSpv.h"

#include "..\SPIRV-Cross\spirv_glsl.hpp"
#include "..\..\..\SPIRV-Cross\spirv_hlsl.hpp"
#include "..\..\..\SPIRV-Cross\spirv_msl.hpp"
#include "VulkanRenderState.h"
#include "Engine\Material\ParameterManager.h"

namespace ma
{

	VulkanTechnique::VulkanTechnique()
	{
	}

	VulkanTechnique::~VulkanTechnique()
	{
	}

	RefPtr<VulkanConstantBuffer> CloneConstBuffer(ConstantBuffer* src)
	{
		RefPtr<VulkanConstantBuffer> pConstantBuffer = CreateConstantBuffer(VS, src->GetBound(), src->GetSize());
		pConstantBuffer->SetName(src->GetName());

		uint32 nUniformCount = src->GetUniformCount();
		for (UINT i = 0; i < nUniformCount; ++i)
		{
			Uniform* pUniform = src->GetUniformByIndex(i);

			RefPtr<Uniform> pUniformCopy = pConstantBuffer->AddUniform(pUniform->GetName());
			pUniformCopy->SetIndex(pUniform->GetIndex());
			pUniformCopy->SetSize(pUniform->GetSize());
			pUniformCopy->SetOffset(pUniform->GetOffset());

			GetParameterManager()->UseDefaultBing(pUniformCopy.get());
		}

		return pConstantBuffer;
	}

	void VulkanTechnique::RT_StreamComplete()
	{
		vks::VulkanDevice* device = GetVulkanDevice();
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		VulkanShaderProgram* pShader = (VulkanShaderProgram*)this->GetShaderProgram();

		m_pPipline = CreateVulkanPipeline(this);

		for (UINT i = 0; i < ShaderType_Number; ++i)
		{
			for (UINT j = 0; j < pShader->GetConstBufferCount((ShaderType)i); ++j)
			{
				ConstantBuffer* pShaderCS = pShader->GetConstBufferByIndex((ShaderType)i,j);
				RefPtr<VulkanConstantBuffer> pConstantBuffer = CloneConstBuffer(pShaderCS);
				pConstantBuffer->SetParent(this);
				this->AddConstBuffer((ShaderType)i,pConstantBuffer.get());
			}
		}

		for (uint32 i = 0; i < pShader->GetSamplerCount(); ++i)
		{
			Uniform* pUniform = pShader->GetSamplerByIndex(i);

			RefPtr<Uniform> pUniformCopy = CreateUniform(pUniform->GetName());
			pUniformCopy->SetTechnique(this);
			pUniformCopy->SetIndex(pUniform->GetIndex());

			this->AddSampler(pUniformCopy.get());

			GetParameterManager()->UseDefaultBing(pUniformCopy.get());
		}

		BindUniform();

		VkResult res;

		//init_descriptor_set
		{
			VkDescriptorSetAllocateInfo alloc_info[1];
			alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info[0].pNext = NULL;
			alloc_info[0].descriptorPool = m_pPipline->m_desc_pool;
			alloc_info[0].descriptorSetCount = 1;
			alloc_info[0].pSetLayouts = &m_pPipline->m_desc_layout[VS];

			res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_descriptorSets[VS]);
			assert(res == VK_SUCCESS);

			std::vector<VkWriteDescriptorSet> vecVSwrite;
			for (uint32 i = 0; i < this->GetConstBufferCount(VS); ++i)
			{
				VulkanConstantBuffer* pConstBuffer = (VulkanConstantBuffer*)this->GetConstBufferByIndex(VS, i);
				VkWriteDescriptorSet write = {};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = NULL;
				write.dstSet = m_descriptorSets[VS];
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				write.pBufferInfo = &pConstBuffer->m_descriptor;
				write.dstArrayElement = 0;
				write.dstBinding = pConstBuffer->GetBound();
				vecVSwrite.push_back(write);
			}

			vkUpdateDescriptorSets(device->logicalDevice, vecVSwrite.size(), vecVSwrite.data(), 0, NULL);
		}

		// PS
		{
			VkDescriptorSetAllocateInfo alloc_info[1];
			alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info[0].pNext = NULL;
			alloc_info[0].descriptorPool = m_pPipline->m_desc_pool;
			alloc_info[0].descriptorSetCount = 1;
			alloc_info[0].pSetLayouts = &m_pPipline->m_desc_layout[PS];

			res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_descriptorSets[PS]);
			assert(res == VK_SUCCESS);

			std::vector<VkWriteDescriptorSet> vecPSwrite;
			for (uint32 i = 0; i < this->GetConstBufferCount(PS); ++i)
			{
				VulkanConstantBuffer* pConstBuffer = (VulkanConstantBuffer*)this->GetConstBufferByIndex(PS, i);
				VkWriteDescriptorSet write = {};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = NULL;
				write.dstSet = m_descriptorSets[PS];
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				write.pBufferInfo = &pConstBuffer->m_descriptor;
				write.dstArrayElement = 0;
				write.dstBinding = pConstBuffer->GetBound();
				vecPSwrite.push_back(write);
			}

			vkUpdateDescriptorSets(device->logicalDevice, vecPSwrite.size(), vecPSwrite.data(), 0, NULL);
		}

		UpdateSamplerDescriptorSets();

		return;
	}

	void VulkanTechnique::UpdateSamplerDescriptorSets()
	{
		if (!m_bSamperDirty)
			return;

		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		{
			VkDescriptorSetAllocateInfo alloc_info[1];
			alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info[0].pNext = NULL;
			alloc_info[0].descriptorPool = m_pPipline->m_desc_pool;
			alloc_info[0].descriptorSetCount = 1;
			alloc_info[0].pSetLayouts = &m_pPipline->m_desc_layout_sampler[0];

			VkResult res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_descriptorSets_sampler[0]);
			assert(res == VK_SUCCESS);

			std::vector<VkWriteDescriptorSet> vec_write;
			for (uint32 i = 0; i < this->GetSamplerCount(); ++i)
			{
				uint32 nIndex = this->GetSamplerByIndex(i)->GetIndex();
				VulkanSamplerStateObject* pSampler = (VulkanSamplerStateObject*)m_arrSampler[nIndex];
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
				write.dstBinding = nIndex;
				vec_write.push_back(write);

			}

			vkUpdateDescriptorSets(device->logicalDevice, vec_write.size(), vec_write.data(), 0, NULL);
		}

		{
			VkDescriptorSetAllocateInfo alloc_info[1];
			alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info[0].pNext = NULL;
			alloc_info[0].descriptorPool = m_pPipline->m_desc_pool;
			alloc_info[0].descriptorSetCount = 1;
			alloc_info[0].pSetLayouts = &m_pPipline->m_desc_layout_sampler[1];

			VkResult res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_descriptorSets_sampler[1]);
			assert(res == VK_SUCCESS);

			std::vector<VkWriteDescriptorSet> vec_write;
			for (uint32 i = 0; i < this->GetSamplerCount(); ++i)
			{
				uint32 nIndex = this->GetSamplerByIndex(i)->GetIndex();
				VulkanSamplerStateObject* pSampler = (VulkanSamplerStateObject*)m_arrSampler[nIndex];
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
				write.dstBinding = nIndex;
				vec_write.push_back(write);

			}

			vkUpdateDescriptorSets(device->logicalDevice, vec_write.size(), vec_write.data(), 0, NULL);
		}

		m_bSamperDirty = false;
	}

	void VulkanTechnique::CommitChanges()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		VulkanShaderProgram* pShader = (VulkanShaderProgram*)this->GetShaderProgram();

		VkCommandBuffer cmdBuffer = pRender->m_drawCmdBuffers;
		if (m_pCommand)
		{
			cmdBuffer = *(VkCommandBuffer*)m_pCommand;
		}

		VkDescriptorSet descriptorSets[4];
		descriptorSets[0] = m_descriptorSets[0];
		descriptorSets[1] = m_descriptorSets[1];
		descriptorSets[2] = m_descriptorSets_sampler[0];
		descriptorSets[3] = m_descriptorSets_sampler[1];

		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipline->m_pipeline);
		vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipline->m_pipelineLayout, 0,
			4, descriptorSets, 0, NULL);

	}
}



