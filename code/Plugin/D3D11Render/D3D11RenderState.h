#pragma once

namespace ma
{
	class D3D11RasterizerStateObject : public RasterizerState
	{
	public:
		D3D11RasterizerStateObject();

		virtual void	RT_StreamComplete();

		static void		Clear();

	public:
		ID3D11RasterizerState* m_pD3D11RSState;
	};

	class D3D11DepthStencilStateObject : public DepthStencilState
	{
	public:
		D3D11DepthStencilStateObject();

		virtual void	RT_StreamComplete(/*UINT32 front_stencil_ref, UINT32 back_stencil_ref*/);


		static void		Clear();

	public:
		ID3D11DepthStencilState* m_pD3D11DSState;
	};

	class D3D11BlendStateObject : public BlendState
	{
	public:
		D3D11BlendStateObject();

		virtual void	RT_StreamComplete(/*const ColourValue& blend_factor, UINT32 sample_mask*/);

		static void		Clear();

	public:
		ID3D11BlendState* m_pD3D11BlendState;
	};
}
