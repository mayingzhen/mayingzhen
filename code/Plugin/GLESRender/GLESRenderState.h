#pragma once

namespace ma
{
	class GLESRasterizerStateObject : public RasterizerState
	{
	public:
		virtual void	RT_StreamComplete() {}
	};

	class GLESDepthStencilStateObject : public DepthStencilState
	{
	public:
		virtual void	RT_StreamComplete() {}
	};

	class GLESBlendStateObject : public BlendState
	{
	public:
		virtual void	RT_StreamComplete() {}
	};
}
