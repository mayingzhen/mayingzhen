#pragma once

namespace ma
{
	class VulkanRasterizerState : public RasterizerState
	{
	public:
		VulkanRasterizerState();

		virtual void	RT_StreamComplete();

	public:
		VkPipelineRasterizationStateCreateInfo rs;
	};

	class VulkanDepthStencilState : public DepthStencilState
	{
	public:
		VulkanDepthStencilState();

		virtual void	RT_StreamComplete();

	public:
		VkPipelineDepthStencilStateCreateInfo ds;
	};

	class VulkanBlendState : public BlendState
	{
	public:
		VulkanBlendState();

		virtual void	RT_StreamComplete();

	public:
		VkPipelineColorBlendStateCreateInfo m_cb;
		VkPipelineColorBlendAttachmentState m_att_state[MAX_RENDERTARGETS];
	};
}
