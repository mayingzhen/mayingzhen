#pragma once


namespace ma
{

	class D3D11Mapping
	{
	public:
		static DXGI_FORMAT	GetD3DIndexType(INDEX_TYPE eType);
		static DXGI_FORMAT  _getPF(PixelFormat Format);
		static D3D11_FILTER  GetD3D11Filter(Filter type);
		static D3D11_TEXTURE_ADDRESS_MODE		GetD3D11Wrap(Wrap type);
		static const char*  GetD3DDeclUsage(DECL_USAGE DeclUsage);
		static DXGI_FORMAT  GetD3DDeclType(DECL_TYPE DeclType);
		static D3D_PRIMITIVE_TOPOLOGY GetD3DPrimitiveType(PRIMITIVE_TYPE type);

		static D3D11_COMPARISON_FUNC get(CompareFunction cf);

		static D3D11_STENCIL_OP get(StencilOperation op, bool invert = false);

		static D3D11_FILL_MODE get(FillMode mode);

		static D3D11_CULL_MODE get(CULL_MODE mode);

		static void GetD3DBlend(BLEND_MODE mode,BOOL& bEnbale,D3D11_BLEND& src,D3D11_BLEND& destm,D3D11_BLEND_OP& op);
		static void GetD3DBlendAlpha(BLEND_MODE mode,BOOL& bEnbale,D3D11_BLEND& src,D3D11_BLEND& destm,D3D11_BLEND_OP& op);

		static PixelFormat _getClosestSupportedPF(PixelFormat ogrePF);
	};

} 


