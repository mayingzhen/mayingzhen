#include "D3D11SamplerState.h"

namespace ma
{


	void D3D11SamplerStateObject::RT_StreamComplete()
	{
		// 		map<SamplerState, ID3D11SamplerState*>::iterator it = m_SamplerStatesPool.find(*pSampler);
// 		if (it != m_SamplerStatesPool.end())
// 		{
// 			return it->second;
// 		}
// 		else
// 		{
// 			ID3D11SamplerState* sample = NULL;
// 
// 			D3D11_SAMPLER_DESC samplerDesc;
// 			memset(&samplerDesc, 0, sizeof samplerDesc);
// 
// 			samplerDesc.Filter = D3D11Mapping::GetD3D11Filter(pSampler->GetFilterMode()); 
// 			samplerDesc.AddressU =  D3D11Mapping::GetD3D11Wrap(pSampler->GetWrapMode());
// 			samplerDesc.AddressV = D3D11Mapping::GetD3D11Wrap(pSampler->GetWrapMode());
// 			samplerDesc.AddressW = D3D11Mapping::GetD3D11Wrap(pSampler->GetWrapModeW());
// 			samplerDesc.MaxAnisotropy = 1;//graphics_->GetTextureAnisotropy();
// 			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
// 			samplerDesc.MinLOD = 0;
// 			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
// 
// 			if (pSampler->GetFilterMode() == TFO_SHADOWCOMPARE)
// 			{
// 				samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;	 
// 			}
// 
// 			m_pD3DDevice->CreateSamplerState(&samplerDesc, &sample);
// 
// 			m_SamplerStatesPool[*pSampler] = sample;
// 
// 			return sample;
// 		}
	}

}