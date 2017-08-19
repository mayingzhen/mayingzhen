#include "VulkanSamplerState.h"

namespace ma
{
	//std::map<SamplerState, IVulkanSamplerState*> g_samplerStatesPool;

	VulkanSamplerStateObject::VulkanSamplerStateObject()
	{
		//m_pImpl = NULL;
	}

	void VulkanSamplerStateObject::RT_StreamComplete()
	{
// 		ASSERT(m_pImpl == NULL);
// 
// 		auto it = g_samplerStatesPool.find(*this);
// 		if (it != g_samplerStatesPool.end())
// 		{
// 			m_pImpl = it->second;
// 		}
// 		else
// 		{
// 			IVulkanSamplerState* sample = NULL;
// 
// 			Vulkan_SAMPLER_DESC samplerDesc;
// 			memset(&samplerDesc, 0, sizeof samplerDesc);
// 
// 			samplerDesc.Filter = VulkanMapping::GetVulkanFilter(this->GetFilterMode()); 
// 			samplerDesc.AddressU =  VulkanMapping::GetVulkanWrap(this->GetWrapMode());
// 			samplerDesc.AddressV = VulkanMapping::GetVulkanWrap(this->GetWrapMode());
// 			samplerDesc.AddressW = VulkanMapping::GetVulkanWrap(this->GetWrapModeW());
// 			samplerDesc.MaxAnisotropy = 1;//graphics_->GetTextureAnisotropy();
// 			samplerDesc.ComparisonFunc = Vulkan_COMPARISON_ALWAYS;
// 			samplerDesc.MinLOD = 0;
// 			samplerDesc.MaxLOD = Vulkan_FLOAT32_MAX;
// 
// 			if (this->GetFilterMode() == TFO_SHADOWCOMPARE)
// 			{
// 				samplerDesc.ComparisonFunc = Vulkan_COMPARISON_LESS;	 
// 			}
// 
// 			GetVulkanDxDevive()->CreateSamplerState(&samplerDesc, &sample);
// 
// 			g_samplerStatesPool.insert(std::make_pair(*this, sample));
// 
// 			m_pImpl = sample;
// 		}
	}

	void VulkanSamplerStateObject::Clear()
	{
// 		for (auto it = g_samplerStatesPool.begin(); it != g_samplerStatesPool.end(); ++it)
// 		{
// 			SAFE_RELEASE(it->second);
// 		}
// 		g_samplerStatesPool.clear();
	}

}