#ifndef  _D3D9Mapping__H__
#define  _D3D9Mapping__H__


// #include <d3d9.h>
// #include <d3dx9.h>
// #include <dxerr.h>
// #include <cg/cg.h>

#include "Engine/RenderSystem/RenderDefine.h"

#define safe_release_com(p) { if (p) { p->Release(); p = NULL; } }

#define DWORD_BIT(x)        (*reinterpret_cast<const DWORD*>(&x))

#define DeclareD3DResource() \
protected: \
    void LostDevice(); \
    void ResetDevice()

#define DXErrorDescription(hr)  DXGetErrorDescription(hr)
#define DXErrorString(hr)       DXGetErrorString(hr)

#define D3DErrorExceptionFunction(func, hr) if (FAILED(hr)) EXCEPTION(std::string("D3D Error: ") + #func + \
                                                                      std::string(" failed. desc: ") + \
                                                                      D3D9Mapping::GetD3DErrorDescription(hr))

namespace ma
{

	struct D3D9RENDER_API D3D9RenderState
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

	struct D3D9RENDER_API D3D9DisplayDevice
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
		static D3DFORMAT    GetD3DFormat(FORMAT Format);
		static void         GetD3D9Filter(FilterOptions Type,DWORD& minFilter,DWORD& magFilter,DWORD& mipFilter);
		static DWORD		GetD3D9Wrap(Wrap type);
		static void         GetD3DDeclUsage(DECL_USAGE DeclUsage,BYTE& d3dUse,BYTE& index);
		static BYTE         GetD3DDeclType(DECL_TYPE DeclType);
		static int          GetD3DDeclTypeSize(BYTE type);
		static D3DPRIMITIVETYPE GetD3DPrimitiveType(PRIMITIVE_TYPE type);
	
		static std::string   GetD3DErrorDescription(HRESULT hr);
		static std::string   GetD3DErrorString(HRESULT hr);

// 		static USAGE        GetUsage(DWORD Usage);
// 		static int          GetLock(DWORD Lock);
// 		static FORMAT       GetFormat(D3DFORMAT D3DFormat);
// 		static DECL_USAGE   GetDeclUsage(BYTE usage);
// 		static DECL_TYPE    GetDeclType(BYTE type);
// 
// 		static std::string   GetUsageString(USAGE usage);
// 		static std::string   GetFormatString(FORMAT format);
	};

} 

#endif

