#pragma once

namespace ma
{
	class D3D11RasterizerStateObject : public RasterizerStateObject
	{
	public:
		explicit D3D11RasterizerStateObject(const RasterizerStateDesc& desc);

		void Active();

		const ID3D11RasterizerState* D3DRasterizerState() const
		{
			return rasterizer_state_;
		}

	private:
		ID3D11RasterizerState* rasterizer_state_;
	};

	class D3D11DepthStencilStateObject : public DepthStencilStateObject
	{
	public:
		explicit D3D11DepthStencilStateObject(const DepthStencilStateDesc& desc);

		void Active(UINT32 front_stencil_ref, UINT32 back_stencil_ref);

		const ID3D11DepthStencilState* D3DDepthStencilState() const
		{
			return depth_stencil_state_;
		}

	private:
		ID3D11DepthStencilState* depth_stencil_state_;
	};

	class D3D11BlendStateObject : public BlendStateObject
	{
	public:
		explicit D3D11BlendStateObject(const BlendStateDesc& desc);

		void Active(const ColourValue& blend_factor, UINT32 sample_mask);

		const ID3D11BlendState* D3DBlendState() const
		{
			return blend_state_;
		}

	private:
		ID3D11BlendState* blend_state_;
	};
}
