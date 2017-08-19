#pragma once

namespace ma
{
	class VulkanSamplerStateObject : public SamplerState
	{
	public:
		VulkanSamplerStateObject();

	public:

		virtual void RT_StreamComplete();

		static void Clear();

	public:
		//IVulkanSamplerState* m_pImpl;
	};

}