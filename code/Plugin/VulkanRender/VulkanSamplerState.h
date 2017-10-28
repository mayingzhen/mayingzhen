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
		VkSampler m_sampler;

		VkDescriptorImageInfo m_descriptor;
	};

}