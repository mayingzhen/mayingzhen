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

		virtual void	RT_StreamComplete(/*UINT32 front_stencil_ref, UINT32 back_stencil_ref*/);


		static void		Clear();

	public:
		VkPipelineDepthStencilStateCreateInfo ds;
	};

	class VulkanBlendStateObject : public BlendState
	{
	public:
		VulkanBlendStateObject();

		virtual void	RT_StreamComplete(/*const ColourValue& blend_factor, UINT32 sample_mask*/);

		static void		Clear();

	public:
		VkPipelineColorBlendStateCreateInfo cb;
	};
}
