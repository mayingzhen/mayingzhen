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
		ds.depthWriteEnable = m_bDepthWrite;
		ds.depthCompareOp = VulkanMapping::get(m_eDepthCheckMode);
	}

	VulkanBlendStateObject::VulkanBlendStateObject()
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

	void VulkanBlendStateObject::RT_StreamComplete()
	{
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
	}
}

