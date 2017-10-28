#pragma once

namespace ma
{
	class VulkanRasterizerStateObject : public RasterizerState
	{
	public:
		VulkanRasterizerStateObject();

		virtual void	RT_StreamComplete();

		static void		Clear();

	public:
		VkPipelineRasterizationStateCreateInfo rs;
	};

	class VulkanDepthStencilStateObject : public DepthStencilState
	{
	public:
		VulkanDepthStencilStateObject();

		virtual void	RT_StreamComplete();


		static void		Clear();

	public:
		VkPipelineDepthStencilStateCreateInfo ds;
	};

	class VulkanBlendStateObject : public BlendState
	{
	public:
		VulkanBlendStateObject();

		virtual void	RT_StreamComplete();

		static void		Clear();

	public:
		VkPipelineColorBlendStateCreateInfo cb;
		VkPipelineColorBlendAttachmentState att_state[1];
	};
}
