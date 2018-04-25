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
		vks::VulkanDevice* device = GetVulkanDevice();

		VulkanTexture* pTex = (VulkanTexture*)this->GetTexture();

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.compareEnable = false;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
		if (GetFilterMode() == TFO_SHADOWCOMPARE)
		{
			samplerCreateInfo.compareEnable = VK_TRUE;
			samplerCreateInfo.compareOp = VK_COMPARE_OP_LESS;
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
		m_descriptor.imageView = pTex->GetImageView(0,0);
		m_descriptor.imageLayout = pTex->m_imageLayout;
	}
}
