#ifndef  _D3D9Mapping__H__
#define  _D3D9Mapping__H__


namespace ma
{

	struct D3D9RenderState
	{
		DWORD       CullMode;
		DWORD       FillMode;
		DWORD       ShadeMode;
		DWORD       ColorWrite;
		DWORD       DepthWrite;
		DWORD       DepthCheck;
		DWORD       DepthFunc;
		DWORD       SrcBlend;
		DWORD       DestBlend;
		DWORD       BlendOp;
		DWORD       AlphaTest;
		DWORD       AlphaFunc;
		DWORD       AlphaRef;
	};

	struct D3D9DisplayDevice
	{
		UINT                    AdapterNumber;
		D3DADAPTER_IDENTIFIER9  Identifier;
		D3DDISPLAYMODE          DisplayMode;
	};

	class D3D9Mapping
	{
	public:
		static DWORD        GetD3DUsage(USAGE Usage);
		static D3DFORMAT	GetD3DIndexType(INDEX_TYPE eType);
		static D3DPOOL      GetD3DPool(USAGE Usage);
		static DWORD        GetD3DLock(int Lock);
		static D3DFORMAT    GetD3DFormat(PixelFormat Format);
		static void         GetD3D9Filter(Filter Type,DWORD& minFilter,DWORD& magFilter,DWORD& mipFilter);
		static DWORD		GetD3D9Wrap(Wrap type);
		static D3DDECLUSAGE GetD3DDeclUsage(DECL_USAGE DeclUsage);
		static BYTE         GetD3DDeclType(DECL_TYPE DeclType);
		static int          GetD3DDeclTypeSize(BYTE type);
		static D3DPRIMITIVETYPE GetD3DPrimitiveType(PRIMITIVE_TYPE type);

	
// 		static std::string   GetD3DErrorDescription(HRESULT hr);
// 		static std::string   GetD3DErrorString(HRESULT hr);

// 		static USAGE        GetUsage(DWORD Usage);
// 		static int          GetLock(DWORD Lock);
// 		static FORMAT       GetFormat(D3DFORMAT D3DFormat);
// 		static DECL_USAGE   GetDeclUsage(BYTE usage);
// 		static DECL_TYPE    GetDeclType(BYTE type);
// 
// 		static std::string   GetUsageString(USAGE usage);
// 		static std::string   GetFormatString(FORMAT format);
	};

	void D3D9Verify( HRESULT hr );

	void D3D9Verify( HRESULT hr, const char *info ) ;

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

#endif

