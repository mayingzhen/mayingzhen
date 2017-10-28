#include "VulkanRenderState.h"



namespace ma
{
	VulkanRasterizerStateObject::VulkanRasterizerStateObject()
	{
		rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rs.pNext = NULL;
		rs.flags = 0;
		rs.polygonMode = VK_POLYGON_MODE_FILL;
		rs.cullMode = VK_CULL_MODE_BACK_BIT;
		rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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

	void VulkanDepthStencilStateObject::RT_StreamComplete()
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
// 		cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
// 		cb.flags = 0;
// 		cb.pNext = NULL;
// 		if (m_bColorWrite)
// 		{
// 			att_state[0].colorWriteMask = 0xf;
// 			att_state[0].blendEnable = VK_FALSE;
// 			att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;
// 			att_state[0].colorBlendOp = VK_BLEND_OP_ADD;
// 			att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
// 			att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
// 			att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
// 			att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
// 			cb.attachmentCount = 1;
// 			cb.pAttachments = att_state;
// 		}
// 
// 		cb.logicOpEnable = VK_FALSE;
// 		cb.logicOp = VK_LOGIC_OP_NO_OP;
// 		cb.blendConstants[0] = 1.0f;
// 		cb.blendConstants[1] = 1.0f;
// 		cb.blendConstants[2] = 1.0f;
// 		cb.blendConstants[3] = 1.0f;
	}

	void VulkanBlendStateObject::RT_StreamComplete()
	{
		cb = {};
		cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		cb.flags = 0;
		cb.pNext = NULL;
		if (m_bColorWrite)
		{
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
		}

		cb.logicOpEnable = VK_FALSE;
		cb.logicOp = VK_LOGIC_OP_NO_OP;
		cb.blendConstants[0] = 1.0f;
		cb.blendConstants[1] = 1.0f;
		cb.blendConstants[2] = 1.0f;
		cb.blendConstants[3] = 1.0f;

	}

	void VulkanBlendStateObject::Clear()
	{
	}
}
