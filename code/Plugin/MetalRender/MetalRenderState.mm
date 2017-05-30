#include "D3D11RenderState.h"

#define CMPVAR(x) if (a.x != b.x) return a.x < b.x;
	

namespace ma
{
	struct CMP_DS_DESC 
	{
		bool operator()(const D3D11_DEPTH_STENCIL_DESC& a, const D3D11_DEPTH_STENCIL_DESC& b) const 
		{
			CMPVAR(DepthEnable);
			CMPVAR(DepthWriteMask);
			CMPVAR(DepthFunc);
			CMPVAR(StencilEnable);
			CMPVAR(StencilReadMask);
			CMPVAR(StencilWriteMask);
			CMPVAR(FrontFace.StencilDepthFailOp);
			CMPVAR(FrontFace.StencilFailOp);
			CMPVAR(FrontFace.StencilFunc);
			CMPVAR(FrontFace.StencilPassOp);
			CMPVAR(BackFace.StencilDepthFailOp);
			CMPVAR(BackFace.StencilFailOp);
			CMPVAR(BackFace.StencilFunc);
			CMPVAR(BackFace.StencilPassOp);

			return false;
		}
	};

	struct CMP_RS_DESC
	{
		bool operator()(const D3D11_RASTERIZER_DESC& a, const D3D11_RASTERIZER_DESC& b) const
		{
			CMPVAR(FillMode);
			CMPVAR(CullMode);
			CMPVAR(FrontCounterClockwise);
			CMPVAR(DepthBias);
			CMPVAR(DepthBiasClamp);
			CMPVAR(SlopeScaledDepthBias);
			CMPVAR(DepthClipEnable);
			CMPVAR(ScissorEnable);
			CMPVAR(MultisampleEnable);
			CMPVAR(AntialiasedLineEnable);

			return false;
		}
	};

	struct CMP_BS_DESC
	{
		bool operator()(const D3D11_BLEND_DESC& a, const D3D11_BLEND_DESC& b) const
		{
			CMPVAR(AlphaToCoverageEnable);
			CMPVAR(IndependentBlendEnable); 
			for (UINT i = 0; i < 8; ++i)
			{
				CMPVAR(RenderTarget[i].BlendEnable);
				CMPVAR(RenderTarget[i].SrcBlend);
				CMPVAR(RenderTarget[i].DestBlend);
				CMPVAR(RenderTarget[i].BlendOp);
				CMPVAR(RenderTarget[i].SrcBlendAlpha);
				CMPVAR(RenderTarget[i].DestBlendAlpha);
				CMPVAR(RenderTarget[i].BlendOpAlpha);
				CMPVAR(RenderTarget[i].RenderTargetWriteMask);
			}

			return false;
		}
	};

	map<D3D11_DEPTH_STENCIL_DESC, ID3D11DepthStencilState*, CMP_DS_DESC> g_depthStatePool;
	map<D3D11_RASTERIZER_DESC, ID3D11RasterizerState*,CMP_RS_DESC> g_rasterizerStatePool;
	map<D3D11_BLEND_DESC, ID3D11BlendState*,CMP_BS_DESC> g_blendStatePool;

	D3D11RasterizerStateObject::D3D11RasterizerStateObject()
	{
		m_pD3D11RSState = NULL;
	}

	void D3D11RasterizerStateObject::RT_StreamComplete()
	{
		ASSERT(m_pD3D11RSState == NULL);

		D3D11_RASTERIZER_DESC stateDesc;
		memset(&stateDesc, 0, sizeof stateDesc);
		stateDesc.FillMode = D3D11Mapping::get(m_eFillMode);
		stateDesc.CullMode = D3D11Mapping::get(m_eCullMode);
		stateDesc.FrontCounterClockwise = TRUE;
		stateDesc.DepthBias = (int)m_fConstantBias;
		stateDesc.DepthBiasClamp = 0.0;
		stateDesc.SlopeScaledDepthBias = m_fSlopeScaleBias;
		stateDesc.DepthClipEnable = TRUE;
		stateDesc.ScissorEnable = m_bScissorTest ? TRUE : FALSE;
		stateDesc.MultisampleEnable = FALSE;
		stateDesc.AntialiasedLineEnable = FALSE;

		auto it = g_rasterizerStatePool.find(stateDesc);
		if (it != g_rasterizerStatePool.end())
		{
			m_pD3D11RSState = it->second;
		}
		else
		{
			ID3D11RasterizerState* newRasterizerState = 0;
			GetD3D11DxDevive()->CreateRasterizerState(&stateDesc, &newRasterizerState);
			if (!newRasterizerState)
			{
				LogError("Failed to create rasterizer state");
			}

			g_rasterizerStatePool.insert(std::make_pair(stateDesc, newRasterizerState));

			m_pD3D11RSState = newRasterizerState;
		}
	}

	void D3D11RasterizerStateObject::Clear()
	{
		for (auto it = g_rasterizerStatePool.begin(); it != g_rasterizerStatePool.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		g_rasterizerStatePool.clear();
	}

	D3D11DepthStencilStateObject::D3D11DepthStencilStateObject()
	{
		m_pD3D11DSState = NULL;
	}

	void D3D11DepthStencilStateObject::RT_StreamComplete(/*UINT32 front_stencil_ref, UINT32 back_stencil_ref*/)
	{
		ASSERT(m_pD3D11DSState == NULL);

		D3D11_DEPTH_STENCIL_DESC stateDesc;
		memset(&stateDesc, 0, sizeof stateDesc);
		stateDesc.DepthEnable = TRUE;
		stateDesc.DepthWriteMask = m_bDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		stateDesc.DepthFunc = D3D11Mapping::get(m_eDepthCheckMode); ;
		stateDesc.StencilEnable = m_bStencil ? TRUE : FALSE;
		stateDesc.StencilReadMask = (unsigned char)m_nStencilMask;
		stateDesc.StencilWriteMask = (unsigned char)m_nStencilWriteMask;
		stateDesc.FrontFace.StencilFailOp = D3D11Mapping::get(m_eStencilFail);
		stateDesc.FrontFace.StencilDepthFailOp = D3D11Mapping::get(m_eDepthFailOp);
		stateDesc.FrontFace.StencilPassOp = D3D11Mapping::get(m_eStencilPass);
		stateDesc.FrontFace.StencilFunc = D3D11Mapping::get(m_eStencilfunc);

		auto it = g_depthStatePool.find(stateDesc);
		if (it != g_depthStatePool.end())
		{
			m_pD3D11DSState = it->second;
		}
		else
		{
			ID3D11DepthStencilState* newDepthState = 0;
			GetD3D11DxDevive()->CreateDepthStencilState(&stateDesc, &newDepthState);
			if (!newDepthState)
			{
				LogError("Failed to create depth state");
				return;
			}

			g_depthStatePool.insert(std::make_pair(stateDesc, newDepthState));

			m_pD3D11DSState = newDepthState;
		}
	}

	void D3D11DepthStencilStateObject::Clear()
	{
		for (auto it = g_depthStatePool.begin(); it != g_depthStatePool.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		g_depthStatePool.clear();
	}


	D3D11BlendStateObject::D3D11BlendStateObject()
	{
		m_pD3D11BlendState = NULL;
	}

	void D3D11BlendStateObject::RT_StreamComplete(/*const ColourValue& blend_factor, UINT32 sample_mask*/)
	{
		ASSERT(m_pD3D11BlendState == NULL);

		D3D11_BLEND_DESC stateDesc;
		memset(&stateDesc, 0, sizeof stateDesc);
		stateDesc.AlphaToCoverageEnable = false;
		stateDesc.IndependentBlendEnable = false;

		D3D11Mapping::GetD3DBlend(m_eBlendMode,
			stateDesc.RenderTarget[0].BlendEnable,
			stateDesc.RenderTarget[0].SrcBlend,
			stateDesc.RenderTarget[0].DestBlend,
			stateDesc.RenderTarget[0].BlendOp);

		D3D11Mapping::GetD3DBlendAlpha(m_eBlendMode,
			stateDesc.RenderTarget[0].BlendEnable,
			stateDesc.RenderTarget[0].SrcBlendAlpha,
			stateDesc.RenderTarget[0].DestBlendAlpha,
			stateDesc.RenderTarget[0].BlendOpAlpha);

		stateDesc.RenderTarget[0].RenderTargetWriteMask = m_bColorWrite ? D3D11_COLOR_WRITE_ENABLE_ALL : 0x0;

		auto it = g_blendStatePool.find(stateDesc);
		if (it != g_blendStatePool.end())
		{
			m_pD3D11BlendState = it->second;
		}
		else
		{
			ID3D11BlendState* newBlendState = 0;
			HRESULT hr = GetD3D11DxDevive()->CreateBlendState(&stateDesc, &newBlendState);
			if (!newBlendState)
			{
				LogError("Failed to create blend state");
			}

			g_blendStatePool.insert(std::make_pair(stateDesc, newBlendState));

			m_pD3D11BlendState = newBlendState;
		}
	}

	void D3D11BlendStateObject::Clear()
	{
		for (auto it = g_blendStatePool.begin(); it != g_blendStatePool.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		g_blendStatePool.clear();
	}
}
