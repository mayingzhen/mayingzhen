#include "VulkanRenderState.h"



namespace ma
{
	VulkanRasterizerState::VulkanRasterizerState()
	{
		rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rs.pNext = NULL;
		rs.flags = 0;
		rs.polygonMode = VK_POLYGON_MODE_FILL;
		rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rs.depthClampEnable = VK_FALSE;
		rs.rasterizerDiscardEnable = VK_FALSE;
		rs.depthBiasEnable = VK_FALSE;
		rs.depthBiasConstantFactor = 0;
		rs.depthBiasClamp = 0;
		rs.depthBiasSlopeFactor = 0;
		rs.lineWidth = 1.0f;

		RT_StreamComplete();
	}

	void VulkanRasterizerState::RT_StreamComplete()
	{
		rs.cullMode = VulkanMapping::get(m_eCullMode);
		rs.depthBiasEnable = m_fConstantBias > 0.0f;
		rs.depthBiasConstantFactor = m_fConstantBias;
		rs.depthBiasSlopeFactor = m_fSlopeScaleBias;
	}

	VulkanDepthStencilState::VulkanDepthStencilState()
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

		RT_StreamComplete();
	}

	void VulkanDepthStencilState::RT_StreamComplete()
	{
		ds.depthWriteEnable = m_bDepthWrite;
		ds.depthCompareOp = VulkanMapping::get(m_eDepthCheckMode);
		ds.depthBoundsTestEnable = m_bDepthBoundsTest;
		
		ds.stencilTestEnable = m_bStencil;
		ds.front.failOp = VulkanMapping::get(m_eStencilFail);
		ds.front.passOp = VulkanMapping::get(m_eStencilPass);
		ds.front.compareOp = VulkanMapping::get(m_eStencilfunc);
		ds.front.compareMask = m_nStencilMask;
		ds.front.reference = m_nStencilRef;
		ds.front.depthFailOp = VulkanMapping::get(m_eDepthFailOp);
		ds.front.writeMask = m_nStencilWriteMask;

		ds.back = ds.front;

		if (m_bBackFaceStencil)
		{
			ds.back.compareOp = VulkanMapping::get(m_eBackStencilfunc);
			ds.back.failOp = VulkanMapping::get(m_eBackStencilFail);
			ds.back.passOp = VulkanMapping::get(m_eBackStencilPass);
			ds.back.depthFailOp = VulkanMapping::get(m_eBackDepthFailOp);
		}
	}

	VulkanBlendState::VulkanBlendState()
	{
		m_cb = {};
		m_cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_cb.flags = 0;
		m_cb.pNext = NULL;

		m_cb.logicOpEnable = VK_FALSE;
		m_cb.logicOp = VK_LOGIC_OP_NO_OP;
		m_cb.blendConstants[0] = 1.0f;
		m_cb.blendConstants[1] = 1.0f;
		m_cb.blendConstants[2] = 1.0f;
		m_cb.blendConstants[3] = 1.0f;

		RT_StreamComplete();
	}

	void VulkanBlendState::RT_StreamComplete()
	{
		for (uint32_t i = 0; i < MAX_RENDERTARGETS; ++i)
		{
			RENDER_TARGET_BLEND_DESC& desc = m_blendDesc[i];

			m_att_state[i].colorWriteMask = desc.nColorWrite;
			m_att_state[i].blendEnable = desc.BlendEnable;
			
			m_att_state[i].colorBlendOp = VulkanMapping::get(desc.BlendOp);
			m_att_state[i].srcColorBlendFactor = VulkanMapping::get(desc.SrcBlend);
			m_att_state[i].dstColorBlendFactor = VulkanMapping::get(desc.DestBlend);

			m_att_state[i].alphaBlendOp = VulkanMapping::get(desc.BlendOpAlpha);
			m_att_state[i].srcAlphaBlendFactor = VulkanMapping::get(desc.SrcBlendAlpha);
			m_att_state[i].dstAlphaBlendFactor = VulkanMapping::get(desc.DestBlendAlpha);
		}

		m_cb.attachmentCount = MAX_RENDERTARGETS;
		m_cb.pAttachments = m_att_state;
	}
}

