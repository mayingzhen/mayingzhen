#pragma once

namespace ma
{
	class VulkanSamplerStateObject : public SamplerState
	{
	public:
		VulkanSamplerStateObject();

		~VulkanSamplerStateObject();

	public:

		virtual void RT_StreamComplete();

	public:
		VkSampler m_sampler = VK_NULL_HANDLE;

		VkDescriptorImageInfo m_descriptor;
	};

}