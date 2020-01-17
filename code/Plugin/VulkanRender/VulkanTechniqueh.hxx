#include "VulkanTechniqueh.h"
#include "VulkanConstantBuffer.h"
#include "glslang/glslang/Include\ResourceLimits.h"
#include "glslang/glslang/Public/ShaderLang.h"
#include "glslang/SPIRV/GlslangToSpv.h"

#include "SPIRV-Cross/spirv_glsl.hpp"
#include "SPIRV-Cross/spirv_hlsl.hpp"
#include "SPIRV-Cross/spirv_msl.hpp"
#include "VulkanRenderState.h"
#include "Engine/Material/ParameterManager.h"

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
		RefPtr<VulkanConstantBuffer> pConstantBuffer = new VulkanConstantBuffer();
		pConstantBuffer->SetBound(src->GetBound());
		pConstantBuffer->SetSize(src->GetSize()); 
		pConstantBuffer->SetName(src->GetName());
		pConstantBuffer->Create();

		uint32_t nUniformCount = src->GetUniformCount();
		for (uint32_t i = 0; i < nUniformCount; ++i)
		{
			Uniform* pUniform = src->GetUniformByIndex(i);

			RefPtr<Uniform> pUniformCopy = pConstantBuffer->AddUniform(pUniform->GetName());
			pUniformCopy->SetShaderType(pUniform->GetShaderType());
			pUniformCopy->SetIndex(pUniform->GetIndex());
			pUniformCopy->SetSize(pUniform->GetSize());
			pUniformCopy->SetOffset(pUniform->GetOffset());
			pUniformCopy->SetMethodBinding(pUniform->GetMethodBinding());
		}

		return pConstantBuffer;
	}

	void VulkanTechnique::RT_StreamComplete()
	{
		VulkanShaderProgram* pShader = (VulkanShaderProgram*)this->GetShaderProgram();
		ASSERT(pShader->IsReady());
		if (!pShader->IsReady())
			return;

		for (uint32_t i = 0; i < ShaderType_Number; ++i)
		{
			ShaderType eType = (ShaderType)i;

			this->ClearConstBuffer(eType);

			for (uint32_t j = 0; j < pShader->GetConstBufferCount(eType); ++j)
			{
				ConstantBuffer* pShaderCS = pShader->GetConstBufferByIndex(eType,j);
				RefPtr<VulkanConstantBuffer> pConstantBuffer = CloneConstBuffer(pShaderCS);
				pConstantBuffer->SetParent(this);

				this->AddConstBuffer(eType,pConstantBuffer.get());
			}

			this->ClearSampler(eType);

			for (uint32_t j = 0; j < pShader->GetSamplerCount(eType); ++j)
			{
				Uniform* pUniform = pShader->GetSamplerByIndex(eType, j);

				RefPtr<Uniform> pUniformCopy = CreateUniform(pUniform->GetName());
				pUniformCopy->SetShaderType(pUniform->GetShaderType());
				pUniformCopy->SetTechnique(this);
				pUniformCopy->SetIndex(pUniform->GetIndex());
				pUniformCopy->SetMethodBinding(pUniform->GetMethodBinding());

				this->AddSampler(eType,pUniformCopy.get());
			}
		}

		this->ClearStorgeBuffer();

		for (uint32_t j = 0; j < pShader->GetStorgeBufferCount(); ++j)
		{
			Uniform* pUniform = pShader->GetStorgeBufferByIndex(j);
			
			RefPtr<Uniform> pUniformCopy = CreateUniform(pUniform->GetName());
			pUniformCopy->SetShaderType(pUniform->GetShaderType());
			pUniformCopy->SetTechnique(this);
			pUniformCopy->SetIndex(pUniform->GetIndex());
			pUniformCopy->SetMethodBinding(pUniform->GetMethodBinding());

			this->AddStorgeBuffer(pUniformCopy.get());
		}
		
		{
			VkDescriptorSetAllocateInfo alloc_info[1];
			alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info[0].pNext = NULL;
			alloc_info[0].descriptorPool = pShader->m_desc_pool;
			alloc_info[0].descriptorSetCount = 1;
			alloc_info[0].pSetLayouts = &pShader->m_graphicPip.m_desc_layout;

			vks::VulkanDevice* device = GetVulkanDevice();
			VkResult res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_grapicDescriptorSet);
			assert(res == VK_SUCCESS);
		}

		if (pShader->m_computePip._Pipeline != VK_NULL_HANDLE)
		{
			VkDescriptorSetAllocateInfo alloc_info[1];
			alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info[0].pNext = NULL;
			alloc_info[0].descriptorPool = pShader->m_desc_pool;
			alloc_info[0].descriptorSetCount = 1;
			alloc_info[0].pSetLayouts = &pShader->m_computePip.m_desc_layout;

			vks::VulkanDevice* device = GetVulkanDevice();
			VkResult res = vkAllocateDescriptorSets(device->logicalDevice, alloc_info, &m_computeDescriptorSet);
			assert(res == VK_SUCCESS);

			UpdateComputeUniformDescriptorSets();
		}

		for (uint32_t i = 0; i < ShaderType_Number; ++i)
		{
			BindUniform(NULL, (ShaderType)i);
		}

		for (uint32_t i = 0; i < ShaderType_Number; ++i)
		{
			ShaderType eType = (ShaderType)i;

			if (eType == CS)
				continue;

			UpdateGrapicUniformDescriptorSets(eType);
		}
	}

	void VulkanTechnique::RT_SetSampler(Uniform* pUniform, SamplerState* pSampler)
	{
		ShaderType eType = pUniform->GetShaderType();
		UpdateGrapicSamplerDescriptorSets(eType, pUniform, pSampler);
	}

	void VulkanTechnique::RT_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer)
	{
		UpdateComputeStogeBufferDescriptorSets(pUniform, pBuffer);
	}

	void VulkanTechnique::UpdateGrapicUniformDescriptorSets(ShaderType eType)
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		VulkanShaderProgram* pShader = (VulkanShaderProgram*)this->GetShaderProgram();

		{
			std::vector<VkWriteDescriptorSet> vecVSwrite;
			for (uint32_t i = 0; i < this->GetConstBufferCount(eType); ++i)
			{
				VulkanConstantBuffer* pConstBuffer = (VulkanConstantBuffer*)this->GetConstBufferByIndex(eType, i);
				VkWriteDescriptorSet write = {};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = NULL;
				write.dstSet = m_grapicDescriptorSet;
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				write.pBufferInfo = &pConstBuffer->m_descriptor;
				write.dstArrayElement = 0;
				write.dstBinding = pConstBuffer->GetBound();
				vecVSwrite.push_back(write);
			}

			vkUpdateDescriptorSets(device->logicalDevice, vecVSwrite.size(), vecVSwrite.data(), 0, NULL);
		}
	}

	void VulkanTechnique::UpdateGrapicSamplerDescriptorSets(ShaderType eType, Uniform* pUniform, SamplerState* pSampler)
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		VulkanShaderProgram* pShader = (VulkanShaderProgram*)this->GetShaderProgram();

		std::vector<VkWriteDescriptorSet> vec_write;
		{
 			VulkanSamplerStateObject* pVkSampler = (VulkanSamplerStateObject*)pSampler;

			pVkSampler->RT_StreamComplete();

			if (pVkSampler->m_descriptor.imageView == 0)
				return;

			VkWriteDescriptorSet writeSampler = {};
			writeSampler.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSampler.pNext = NULL;
			writeSampler.dstSet = m_grapicDescriptorSet;
			writeSampler.descriptorCount = 1;
			writeSampler.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
			writeSampler.pImageInfo = &pVkSampler->m_descriptor;
			writeSampler.dstArrayElement = 0;
			writeSampler.dstBinding = pUniform->GetIndex() + pShader->m_samplershiftBinding[eType];
			vec_write.push_back(writeSampler);

			VkWriteDescriptorSet writeTexture = {};
			writeTexture.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeTexture.pNext = NULL;
			writeTexture.dstSet = m_grapicDescriptorSet;
			writeTexture.descriptorCount = 1;
			writeTexture.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			writeTexture.pImageInfo = &pVkSampler->m_descriptor;
			writeTexture.dstArrayElement = 0;
			writeTexture.dstBinding = pUniform->GetIndex() + pShader->m_texshiftBinding[eType];
			vec_write.push_back(writeTexture);

		}

		vkUpdateDescriptorSets(device->logicalDevice, vec_write.size(), vec_write.data(), 0, NULL);
	}

	void VulkanTechnique::UpdateComputeUniformDescriptorSets()
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();

		VulkanShaderProgram* pShader = (VulkanShaderProgram*)this->GetShaderProgram();

		{
			std::vector<VkWriteDescriptorSet> vecVSwrite;
			for (uint32_t i = 0; i < this->GetConstBufferCount(CS); ++i)
			{
				VulkanConstantBuffer* pConstBuffer = (VulkanConstantBuffer*)this->GetConstBufferByIndex(CS, i);
				VkWriteDescriptorSet write = {};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = NULL;
				write.dstSet = m_computeDescriptorSet;
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				write.pBufferInfo = &pConstBuffer->m_descriptor;
				write.dstArrayElement = 0;
				write.dstBinding = pConstBuffer->GetBound();
				vecVSwrite.push_back(write);
			}

			vkUpdateDescriptorSets(device->logicalDevice, vecVSwrite.size(), vecVSwrite.data(), 0, NULL);
		}
	}

	void VulkanTechnique::UpdateComputeStogeBufferDescriptorSets(Uniform* pUniform, HardwareBuffer* pBuffer)
	{
		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanShaderProgram* pShader = (VulkanShaderProgram*)this->GetShaderProgram();

		VulkanVertexBuffer* pVKBuffer = dynamic_cast<VulkanVertexBuffer*>(pBuffer);
		if (pVKBuffer == nullptr)
			return;

		vks::Buffer storageBuffer = pVKBuffer->m_vertexBuffer;
		std::vector<VkWriteDescriptorSet> vec_write;
		VkWriteDescriptorSet write = vks::initializers::writeDescriptorSet(
				m_computeDescriptorSet,
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				0,
				&storageBuffer.descriptor);
		write.dstBinding = pUniform->GetIndex();
		vec_write.push_back(write);

		vkUpdateDescriptorSets(device->logicalDevice, vec_write.size(), vec_write.data(), 0, NULL);
	}
}






