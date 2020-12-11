#include "VulkanSamplerState.h"

namespace ma
{
	VulkanSamplerStateObject::VulkanSamplerStateObject()
	{

	}

	VulkanSamplerStateObject::~VulkanSamplerStateObject()
	{
		if (m_sampler != VK_NULL_HANDLE)
		{
			vks::VulkanDevice* device = GetVulkanDevice();
			vkDestroySampler(device->logicalDevice, m_sampler, nullptr);
		}
	}

	void VulkanSamplerStateObject::RT_StreamComplete()
	{
		if (m_sampler != VK_NULL_HANDLE)
			return;

		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanTexture* pTex = (VulkanTexture*)this->GetTexture();

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.addressModeU = VulkanMapping::GetWrap(m_eWrap);
		samplerCreateInfo.addressModeV = VulkanMapping::GetWrap(m_eWrap);
		samplerCreateInfo.addressModeW = VulkanMapping::GetWrap(m_eWrapW);
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
		samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerCreateInfo.compareEnable = false;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
		if (m_eFilter == TFO_SHADOWCOMPARE)
		{
			samplerCreateInfo.compareEnable = VK_TRUE;
			samplerCreateInfo.compareOp = VK_COMPARE_OP_LESS;
		}
		else if (m_eFilter == TFO_POINT)
		{
			samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
		else if (m_eFilter == TFO_BILINEAR)
		{
			samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
		else if (m_eFilter == TFO_TRILINEAR)
		{
			samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}

		samplerCreateInfo.minLod = 0.0f;
		// Max level-of-detail should match mip level count
		samplerCreateInfo.maxLod = /*(useStaging) ? */(float)pTex->GetMipMapNumber()/* : 0.0f*/;
		// Enable anisotropic filtering
		samplerCreateInfo.maxAnisotropy = 1.0;
		samplerCreateInfo.anisotropyEnable = VK_TRUE;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		VK_CHECK_RESULT(vkCreateSampler(device->logicalDevice, &samplerCreateInfo, nullptr, &m_sampler));

		m_descriptor.sampler = m_sampler;
		m_descriptor.imageView = pTex->GetShaderView();
		m_descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		if (pTex->GetUsage() == TEXTURE_USAGE::USAGE_DEPTHSTENCIL)
		{
			m_descriptor.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		}
	}
}
