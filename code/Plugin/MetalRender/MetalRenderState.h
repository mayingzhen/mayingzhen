#pragma once

#import <Metal/Metal.h>

namespace ma
{
	class MetalRasterizerStateObject : public RasterizerState
	{
	public:
		MetalRasterizerStateObject();

		virtual void	RT_StreamComplete();

		static void		Clear();

	public:
		
	};

	class MetalDepthStencilStateObject : public DepthStencilState
	{
	public:
		MetalDepthStencilStateObject();

		virtual void	RT_StreamComplete();


		static void		Clear();

	public:
		id<MTLDepthStencilState> m_pMetalDSState;
	};

	class MetalBlendStateObject : public BlendState
	{
	public:
		MetalBlendStateObject();

		virtual void	RT_StreamComplete();

		static void		Clear();

	public:
		
	};
}
