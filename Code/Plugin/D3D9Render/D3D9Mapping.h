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
		static DWORD        GetD3DFilterType(TEXTURE_FILTER_TYPE Type);
		static DWORD        GetD3DXFilter(int filter);
		static DWORD        GetD3DAddressMode(TEXTURE_ADDRESS_MODE Mode);
		static BYTE         GetD3DDeclUsage(DECL_USAGE DeclUsage);
		static BYTE         GetD3DDeclType(DECL_TYPE DeclType);
		static int          GetD3DDeclTypeSize(BYTE type);
		static D3DPRIMITIVETYPE GetD3DPrimitiveType(PRIMITIVE_TYPE type);
		//static CGprofile    GetCgProfile(SHADER_PROFILE Profile);

		static std::string   GetHLSLProfile(SHADER_PROFILE Profile);

		static int          GetTypeSize(SHADER_PARAM_TYPE Type);

		static D3DXIMAGE_FILEFORMAT GetD3DXImageFileFormat(IMAGE_FILE_FORMAT Format);

		static std::string   GetD3DErrorDescription(HRESULT hr);
		static std::string   GetD3DErrorString(HRESULT hr);

		static USAGE        GetUsage(DWORD Usage);
		static int          GetLock(DWORD Lock);
		static FORMAT       GetFormat(D3DFORMAT D3DFormat);
		static TEXTURE_FILTER_TYPE GetFilterType(DWORD Type);
		static TEXTURE_ADDRESS_MODE GetAddressMode(DWORD Mode);
		static DECL_USAGE   GetDeclUsage(BYTE usage);
		static DECL_TYPE    GetDeclType(BYTE type);

		static D3DMULTISAMPLE_TYPE GetD3DMultiSampleType(MULTI_SAMPLE ms);
		static MULTI_SAMPLE GetMultiSampleType(D3DMULTISAMPLE_TYPE ms);

		static std::string   GetMSAAString(MULTI_SAMPLE msaa);
		static std::string   GetUsageString(USAGE usage);
		static std::string   GetFormatString(FORMAT format);
		static std::string   GetShaderProfileString(SHADER_PROFILE profile);
	};

} 

#endif
