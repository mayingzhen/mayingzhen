#pragma once


namespace ma
{

	class D3D11Mapping
	{
	public:
		//static D3D11_USAGE  GetD3DTextureUsage(TEXTURE_USAGE Usage);
		//static D3D11_USAGE  GetD3DBufferUsage(HBU_USAGE Usage);
		static DXGI_FORMAT	GetD3DIndexType(INDEX_TYPE eType);
		//static D3DPOOL      GetD3DPool(HBU_USAGE Usage);
		//static DWORD        GetD3DLock(int Lock);
		static DXGI_FORMAT  _getPF(PixelFormat Format);
		static D3D11_FILTER         GetD3D11Filter(Filter type);
		static D3D11_TEXTURE_ADDRESS_MODE		GetD3D11Wrap(Wrap type);
		static const char*  GetD3DDeclUsage(DECL_USAGE DeclUsage);
		static DXGI_FORMAT  GetD3DDeclType(DECL_TYPE DeclType);
		//static int          GetD3DDeclTypeSize(BYTE type);
		static D3D_PRIMITIVE_TOPOLOGY GetD3DPrimitiveType(PRIMITIVE_TYPE type);

		static D3D11_COMPARISON_FUNC get(CompareFunction cf);

		static D3D11_STENCIL_OP get(StencilOperation op, bool invert = false);

		static D3D11_FILL_MODE get(FillMode mode);

		static D3D11_CULL_MODE get(CULL_MODE mode);

		static void GetD3DBlend(BLEND_MODE mode,BOOL& bEnbale,D3D11_BLEND& src,D3D11_BLEND& destm,D3D11_BLEND_OP& op);

		static PixelFormat _getClosestSupportedPF(PixelFormat ogrePF);
	};

	void D3D11Verify( HRESULT hr );

	void D3D11Verify( HRESULT hr, const char *info ) ;

	RECT toD3DRECT(const Box &lockBox)
	{
		RECT prect;
		ASSERT(lockBox.getDepth() == 1);
		prect.left = static_cast<LONG>(lockBox.left);
		prect.right = static_cast<LONG>(lockBox.right);
		prect.top = static_cast<LONG>(lockBox.top);
		prect.bottom = static_cast<LONG>(lockBox.bottom);
		return prect;
	}
} 


