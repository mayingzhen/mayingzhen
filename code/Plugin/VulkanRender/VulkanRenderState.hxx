#include "VulkanRenderState.h"

#define CMPVAR(x) if (a.x != b.x) return a.x < b.x;
	

namespace ma
{
// 	struct CMP_DS_DESC 
// 	{
// 		bool operator()(const Vulkan_DEPTH_STENCIL_DESC& a, const Vulkan_DEPTH_STENCIL_DESC& b) const 
// 		{
// 			CMPVAR(DepthEnable);
// 			CMPVAR(DepthWriteMask);
// 			CMPVAR(DepthFunc);
// 			CMPVAR(StencilEnable);
// 			CMPVAR(StencilReadMask);
// 			CMPVAR(StencilWriteMask);
// 			CMPVAR(FrontFace.StencilDepthFailOp);
// 			CMPVAR(FrontFace.StencilFailOp);
// 			CMPVAR(FrontFace.StencilFunc);
// 			CMPVAR(FrontFace.StencilPassOp);
// 			CMPVAR(BackFace.StencilDepthFailOp);
// 			CMPVAR(BackFace.StencilFailOp);
// 			CMPVAR(BackFace.StencilFunc);
// 			CMPVAR(BackFace.StencilPassOp);
// 
// 			return false;
// 		}
// 	};

// 	struct CMP_RS_DESC
// 	{
// 		bool operator()(const Vulkan_RASTERIZER_DESC& a, const Vulkan_RASTERIZER_DESC& b) const
// 		{
// 			CMPVAR(FillMode);
// 			CMPVAR(CullMode);
// 			CMPVAR(FrontCounterClockwise);
// 			CMPVAR(DepthBias);
// 			CMPVAR(DepthBiasClamp);
// 			CMPVAR(SlopeScaledDepthBias);
// 			CMPVAR(DepthClipEnable);
// 			CMPVAR(ScissorEnable);
// 			CMPVAR(MultisampleEnable);
// 			CMPVAR(AntialiasedLineEnable);
// 
// 			return false;
// 		}
// 	};

// 	struct CMP_BS_DESC
// 	{
// 		bool operator()(const Vulkan_BLEND_DESC& a, const Vulkan_BLEND_DESC& b) const
// 		{
// 			CMPVAR(AlphaToCoverageEnable);
// 			CMPVAR(IndependentBlendEnable); 
// 			for (UINT i = 0; i < 8; ++i)
// 			{
// 				CMPVAR(RenderTarget[i].BlendEnable);
// 				CMPVAR(RenderTarget[i].SrcBlend);
// 				CMPVAR(RenderTarget[i].DestBlend);
// 				CMPVAR(RenderTarget[i].BlendOp);
// 				CMPVAR(RenderTarget[i].SrcBlendAlpha);
// 				CMPVAR(RenderTarget[i].DestBlendAlpha);
// 				CMPVAR(RenderTarget[i].BlendOpAlpha);
// 				CMPVAR(RenderTarget[i].RenderTargetWriteMask);
// 			}
// 
// 			return false;
// 		}
// 	};

// 	map<Vulkan_DEPTH_STENCIL_DESC, IVulkanDepthStencilState*, CMP_DS_DESC> g_depthStatePool;
// 	map<Vulkan_RASTERIZER_DESC, IVulkanRasterizerState*,CMP_RS_DESC> g_rasterizerStatePool;
// 	map<Vulkan_BLEND_DESC, IVulkanBlendState*,CMP_BS_DESC> g_blendStatePool;

	VulkanRasterizerStateObject::VulkanRasterizerStateObject()
	{
		rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rs.pNext = NULL;
		rs.flags = 0;
		rs.polygonMode = VK_POLYGON_MODE_FILL;
		rs.cullMode = VK_CULL_MODE_BACK_BIT;
		rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rs.depthClampEnable = VK_FALSE;
		rs.rasterizerDiscardEnable = VK_FALSE;
		rs.depthBiasEnable = VK_FALSE;
		rs.depthBiasConstantFactor = 0;
		rs.depthBiasClamp = 0;
		rs.depthBiasSlopeFactor = 0;
		rs.lineWidth = 1.0f;
	}

	void VulkanRasterizerStateObject::RT_StreamComplete()
	{
	}

	void VulkanRasterizerStateObject::Clear()
	{
// 		for (auto it = g_rasterizerStatePool.begin(); it != g_rasterizerStatePool.end(); ++it)
// 		{
// 			SAFE_RELEASE(it->second);
// 		}
// 		g_rasterizerStatePool.clear();
	}

	VulkanDepthStencilStateObject::VulkanDepthStencilStateObject()
	{
		ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		ds.pNext = NULL;
		ds.flags = 0;
		ds.depthTestEnable = true/*include_depth*/;
		ds.depthWriteEnable = true/*include_depth*/;
		ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		ds.depthBoundsTestEnable = VK_FALSE;
		ds.stencilTestEnable = VK_FALSE;
		ds.back.failOp = VK_STENCIL_OP_KEEP;
		ds.back.passOp = VK_STENCIL_OP_KEEP;
		ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
		ds.back.compareMask = 0;
		ds.back.reference = 0;
		ds.back.depthFailOp = VK_STENCIL_OP_KEEP;
		ds.back.writeMask = 0;
		ds.minDepthBounds = 0;
		ds.maxDepthBounds = 0;
		ds.stencilTestEnable = VK_FALSE;
		ds.front = ds.back;
	}

	void VulkanDepthStencilStateObject::RT_StreamComplete(/*UINT32 front_stencil_ref, UINT32 back_stencil_ref*/)
	{
// 		ASSERT(m_pVulkanDSState == NULL);
// 
// 		Vulkan_DEPTH_STENCIL_DESC stateDesc;
// 		memset(&stateDesc, 0, sizeof stateDesc);
// 		stateDesc.DepthEnable = TRUE;
// 		stateDesc.DepthWriteMask = m_bDepthWrite ? Vulkan_DEPTH_WRITE_MASK_ALL : Vulkan_DEPTH_WRITE_MASK_ZERO;
// 		stateDesc.DepthFunc = VulkanMapping::get(m_eDepthCheckMode); ;
// 		stateDesc.StencilEnable = m_bStencil ? TRUE : FALSE;
// 		stateDesc.StencilReadMask = (unsigned char)m_nStencilMask;
// 		stateDesc.StencilWriteMask = (unsigned char)m_nStencilWriteMask;
// 		stateDesc.FrontFace.StencilFailOp = VulkanMapping::get(m_eStencilFail);
// 		stateDesc.FrontFace.StencilDepthFailOp = VulkanMapping::get(m_eDepthFailOp);
// 		stateDesc.FrontFace.StencilPassOp = VulkanMapping::get(m_eStencilPass);
// 		stateDesc.FrontFace.StencilFunc = VulkanMapping::get(m_eStencilfunc);
// 
// 		auto it = g_depthStatePool.find(stateDesc);
// 		if (it != g_depthStatePool.end())
// 		{
// 			m_pVulkanDSState = it->second;
// 		}
// 		else
// 		{
// 			IVulkanDepthStencilState* newDepthState = 0;
// 			GetVulkanDxDevive()->CreateDepthStencilState(&stateDesc, &newDepthState);
// 			if (!newDepthState)
// 			{
// 				LogError("Failed to create depth state");
// 				return;
// 			}
// 
// 			g_depthStatePool.insert(std::make_pair(stateDesc, newDepthState));
// 
// 			m_pVulkanDSState = newDepthState;
// 		}
	}

	void VulkanDepthStencilStateObject::Clear()
	{
// 		for (auto it = g_depthStatePool.begin(); it != g_depthStatePool.end(); ++it)
// 		{
// 			SAFE_RELEASE(it->second);
// 		}
// 		g_depthStatePool.clear();
	}


	VulkanBlendStateObject::VulkanBlendStateObject()
	{
		cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		cb.flags = 0;
		cb.pNext = NULL;
		VkPipelineColorBlendAttachmentState att_state[1];
		att_state[0].colorWriteMask = 0xf;
		att_state[0].blendEnable = VK_FALSE;
		att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;
		att_state[0].colorBlendOp = VK_BLEND_OP_ADD;
		att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		cb.attachmentCount = 1;
		cb.pAttachments = att_state;
		cb.logicOpEnable = VK_FALSE;
		cb.logicOp = VK_LOGIC_OP_NO_OP;
		cb.blendConstants[0] = 1.0f;
		cb.blendConstants[1] = 1.0f;
		cb.blendConstants[2] = 1.0f;
		cb.blendConstants[3] = 1.0f;
		//m_pVulkanBlendState = NULL;
	}

	void VulkanBlendStateObject::RT_StreamComplete(/*const ColourValue& blend_factor, UINT32 sample_mask*/)
	{
// 		ASSERT(m_pVulkanBlendState == NULL);
// 
// 		Vulkan_BLEND_DESC stateDesc;
// 		memset(&stateDesc, 0, sizeof stateDesc);
// 		stateDesc.AlphaToCoverageEnable = false;
// 		stateDesc.IndependentBlendEnable = false;
// 
// 		VulkanMapping::GetD3DBlend(m_eBlendMode,
// 			stateDesc.RenderTarget[0].BlendEnable,
// 			stateDesc.RenderTarget[0].SrcBlend,
// 			stateDesc.RenderTarget[0].DestBlend,
// 			stateDesc.RenderTarget[0].BlendOp);
// 
// 		VulkanMapping::GetD3DBlendAlpha(m_eBlendMode,
// 			stateDesc.RenderTarget[0].BlendEnable,
// 			stateDesc.RenderTarget[0].SrcBlendAlpha,
// 			stateDesc.RenderTarget[0].DestBlendAlpha,
// 			stateDesc.RenderTarget[0].BlendOpAlpha);
// 
// 		stateDesc.RenderTarget[0].RenderTargetWriteMask = m_bColorWrite ? Vulkan_COLOR_WRITE_ENABLE_ALL : 0x0;
// 
// 		auto it = g_blendStatePool.find(stateDesc);
// 		if (it != g_blendStatePool.end())
// 		{
// 			m_pVulkanBlendState = it->second;
// 		}
// 		else
// 		{
// 			IVulkanBlendState* newBlendState = 0;
// 			HRESULT hr = GetVulkanDxDevive()->CreateBlendState(&stateDesc, &newBlendState);
// 			if (!newBlendState)
// 			{
// 				LogError("Failed to create blend state");
// 			}
// 
// 			g_blendStatePool.insert(std::make_pair(stateDesc, newBlendState));
// 
// 			m_pVulkanBlendState = newBlendState;
// 		}
	}

	void VulkanBlendStateObject::Clear()
	{
// 		for (auto it = g_blendStatePool.begin(); it != g_blendStatePool.end(); ++it)
// 		{
// 			SAFE_RELEASE(it->second);
// 		}
// 		g_blendStatePool.clear();
	}
}
