#include "D3D11SamplerState.h"

namespace ma
{
	std::map<SamplerState, ID3D11SamplerState*> g_samplerStatesPool;

	D3D11SamplerStateObject::D3D11SamplerStateObject()
	{
		m_pImpl = NULL;
	}

	void D3D11SamplerStateObject::RT_StreamComplete()
	{
		ASSERT(m_pImpl == NULL);

		auto it = g_samplerStatesPool.find(*this);
		if (it != g_samplerStatesPool.end())
		{
			m_pImpl = it->second;
		}
		else
		{
			ID3D11SamplerState* sample = NULL;

			D3D11_SAMPLER_DESC samplerDesc;
			memset(&samplerDesc, 0, sizeof samplerDesc);

			samplerDesc.Filter = D3D11Mapping::GetD3D11Filter(this->GetFilterMode()); 
			samplerDesc.AddressU =  D3D11Mapping::GetD3D11Wrap(this->GetWrapMode());
			samplerDesc.AddressV = D3D11Mapping::GetD3D11Wrap(this->GetWrapMode());
			samplerDesc.AddressW = D3D11Mapping::GetD3D11Wrap(this->GetWrapModeW());
			samplerDesc.MaxAnisotropy = 1;//graphics_->GetTextureAnisotropy();
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			if (this->GetFilterMode() == TFO_SHADOWCOMPARE)
			{
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;	 
			}

			GetD3D11DxDevive()->CreateSamplerState(&samplerDesc, &sample);

			g_samplerStatesPool.insert(std::make_pair(*this, sample));

			m_pImpl = sample;
		}
	}

	void D3D11SamplerStateObject::Clear()
	{
		for (auto it = g_samplerStatesPool.begin(); it != g_samplerStatesPool.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		g_samplerStatesPool.clear();
	}

}