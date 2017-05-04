#pragma once

namespace ma
{
	class GLESRasterizerStateObject : public RasterizerState
	{
	public:

		virtual void	RT_StreamComplete() {}

	public:
	};

	class GLESDepthStencilStateObject : public DepthStencilState
	{
	public:

		virtual void	RT_StreamComplete(/*UINT32 front_stencil_ref, UINT32 back_stencil_ref*/) {}


	public:
	};

	class GLESBlendStateObject : public BlendState
	{
	public:
		virtual void	RT_StreamComplete(/*const ColourValue& blend_factor, UINT32 sample_mask*/) {}

	public:
	};
}
