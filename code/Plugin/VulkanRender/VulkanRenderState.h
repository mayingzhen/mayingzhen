#pragma once

namespace ma
{
	class VulkanRasterizerStateObject : public RasterizerState
	{
	public:
		VulkanRasterizerStateObject();

		virtual void	RT_StreamComplete();

	public:
		VkPipelineRasterizationStateCreateInfo rs;
	};

	class VulkanDepthStencilStateObject : public DepthStencilState
	{
	public:
		VulkanDepthStencilStateObject();

		virtual void	RT_StreamComplete();

	public:
		VkPipelineDepthStencilStateCreateInfo ds;
	};

	class VulkanBlendStateObject : public BlendState
	{
	public:
		VulkanBlendStateObject();

		virtual void	RT_StreamComplete();

	public:
		VkPipelineColorBlendStateCreateInfo m_cb;
		VkPipelineColorBlendAttachmentState m_att_state[MAX_RENDERTARGETS];
	};
}
