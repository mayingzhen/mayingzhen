#include "VulkanRenderState.h"

#define CMPVAR(x) if (a.x != b.x) return a.x < b.x;
	

namespace ma
{
	struct CMP_DS_DESC 
	{
		bool operator()(const Vulkan_DEPTH_STENCIL_DESC& a, const Vulkan_DEPTH_STENCIL_DESC& b) const 
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
		bool operator()(const Vulkan_RASTERIZER_DESC& a, const Vulkan_RASTERIZER_DESC& b) const
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
		bool operator()(const Vulkan_BLEND_DESC& a, const Vulkan_BLEND_DESC& b) const
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

	map<Vulkan_DEPTH_STENCIL_DESC, IVulkanDepthStencilState*, CMP_DS_DESC> g_depthStatePool;
	map<Vulkan_RASTERIZER_DESC, IVulkanRasterizerState*,CMP_RS_DESC> g_rasterizerStatePool;
	map<Vulkan_BLEND_DESC, IVulkanBlendState*,CMP_BS_DESC> g_blendStatePool;

	VulkanRasterizerStateObject::VulkanRasterizerStateObject()
	{
		m_pVulkanRSState = NULL;
	}

	void VulkanRasterizerStateObject::RT_StreamComplete()
	{
		ASSERT(m_pVulkanRSState == NULL);

		Vulkan_RASTERIZER_DESC stateDesc;
		memset(&stateDesc, 0, sizeof stateDesc);
		stateDesc.FillMode = VulkanMapping::get(m_eFillMode);
		stateDesc.CullMode = VulkanMapping::get(m_eCullMode);
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
			m_pVulkanRSState = it->second;
		}
		else
		{
			IVulkanRasterizerState* newRasterizerState = 0;
			GetVulkanDxDevive()->CreateRasterizerState(&stateDesc, &newRasterizerState);
			if (!newRasterizerState)
			{
				LogError("Failed to create rasterizer state");
			}

			g_rasterizerStatePool.insert(std::make_pair(stateDesc, newRasterizerState));

			m_pVulkanRSState = newRasterizerState;
		}
	}

	void VulkanRasterizerStateObject::Clear()
	{
		for (auto it = g_rasterizerStatePool.begin(); it != g_rasterizerStatePool.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		g_rasterizerStatePool.clear();
	}

	VulkanDepthStencilStateObject::VulkanDepthStencilStateObject()
	{
		m_pVulkanDSState = NULL;
	}

	void VulkanDepthStencilStateObject::RT_StreamComplete(/*UINT32 front_stencil_ref, UINT32 back_stencil_ref*/)
	{
		ASSERT(m_pVulkanDSState == NULL);

		Vulkan_DEPTH_STENCIL_DESC stateDesc;
		memset(&stateDesc, 0, sizeof stateDesc);
		stateDesc.DepthEnable = TRUE;
		stateDesc.DepthWriteMask = m_bDepthWrite ? Vulkan_DEPTH_WRITE_MASK_ALL : Vulkan_DEPTH_WRITE_MASK_ZERO;
		stateDesc.DepthFunc = VulkanMapping::get(m_eDepthCheckMode); ;
		stateDesc.StencilEnable = m_bStencil ? TRUE : FALSE;
		stateDesc.StencilReadMask = (unsigned char)m_nStencilMask;
		stateDesc.StencilWriteMask = (unsigned char)m_nStencilWriteMask;
		stateDesc.FrontFace.StencilFailOp = VulkanMapping::get(m_eStencilFail);
		stateDesc.FrontFace.StencilDepthFailOp = VulkanMapping::get(m_eDepthFailOp);
		stateDesc.FrontFace.StencilPassOp = VulkanMapping::get(m_eStencilPass);
		stateDesc.FrontFace.StencilFunc = VulkanMapping::get(m_eStencilfunc);

		auto it = g_depthStatePool.find(stateDesc);
		if (it != g_depthStatePool.end())
		{
			m_pVulkanDSState = it->second;
		}
		else
		{
			IVulkanDepthStencilState* newDepthState = 0;
			GetVulkanDxDevive()->CreateDepthStencilState(&stateDesc, &newDepthState);
			if (!newDepthState)
			{
				LogError("Failed to create depth state");
				return;
			}

			g_depthStatePool.insert(std::make_pair(stateDesc, newDepthState));

			m_pVulkanDSState = newDepthState;
		}
	}

	void VulkanDepthStencilStateObject::Clear()
	{
		for (auto it = g_depthStatePool.begin(); it != g_depthStatePool.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		g_depthStatePool.clear();
	}


	VulkanBlendStateObject::VulkanBlendStateObject()
	{
		m_pVulkanBlendState = NULL;
	}

	void VulkanBlendStateObject::RT_StreamComplete(/*const ColourValue& blend_factor, UINT32 sample_mask*/)
	{
		ASSERT(m_pVulkanBlendState == NULL);

		Vulkan_BLEND_DESC stateDesc;
		memset(&stateDesc, 0, sizeof stateDesc);
		stateDesc.AlphaToCoverageEnable = false;
		stateDesc.IndependentBlendEnable = false;

		VulkanMapping::GetD3DBlend(m_eBlendMode,
			stateDesc.RenderTarget[0].BlendEnable,
			stateDesc.RenderTarget[0].SrcBlend,
			stateDesc.RenderTarget[0].DestBlend,
			stateDesc.RenderTarget[0].BlendOp);

		VulkanMapping::GetD3DBlendAlpha(m_eBlendMode,
			stateDesc.RenderTarget[0].BlendEnable,
			stateDesc.RenderTarget[0].SrcBlendAlpha,
			stateDesc.RenderTarget[0].DestBlendAlpha,
			stateDesc.RenderTarget[0].BlendOpAlpha);

		stateDesc.RenderTarget[0].RenderTargetWriteMask = m_bColorWrite ? Vulkan_COLOR_WRITE_ENABLE_ALL : 0x0;

		auto it = g_blendStatePool.find(stateDesc);
		if (it != g_blendStatePool.end())
		{
			m_pVulkanBlendState = it->second;
		}
		else
		{
			IVulkanBlendState* newBlendState = 0;
			HRESULT hr = GetVulkanDxDevive()->CreateBlendState(&stateDesc, &newBlendState);
			if (!newBlendState)
			{
				LogError("Failed to create blend state");
			}

			g_blendStatePool.insert(std::make_pair(stateDesc, newBlendState));

			m_pVulkanBlendState = newBlendState;
		}
	}

	void VulkanBlendStateObject::Clear()
	{
		for (auto it = g_blendStatePool.begin(); it != g_blendStatePool.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		g_blendStatePool.clear();
	}
}
