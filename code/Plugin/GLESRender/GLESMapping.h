#ifndef  _GLESMapping__H__
#define  _GLESMapping__H__


#include "Engine/RenderSystem/RenderDefine.h"


namespace ma
{

	struct GLESRENDER_API GLESRenderState
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

	struct  GLESDisplayDevice
	{

	};

	class GLESMapping
	{
	public:
// 		static DWORD        GetD3DUsage(USAGE Usage);
// 		static D3DPOOL      GetD3DPool(USAGE Usage);
// 		static DWORD        GetD3DLock(int Lock);
// 		static D3DFORMAT    GetD3DFormat(FORMAT Format);
// 		static DWORD        GetD3DFilterType(TEXTURE_FILTER_TYPE Type);
// 		static DWORD        GetD3DXFilter(int filter);
// 		static DWORD        GetD3DAddressMode(TEXTURE_ADDRESS_MODE Mode);
// 		static BYTE         GetD3DDeclUsage(DECL_USAGE DeclUsage);
// 		static BYTE         GetD3DDeclType(DECL_TYPE DeclType);
// 		static int          GetD3DDeclTypeSize(BYTE type);
		static GLenum GetGLESUSage(USAGE Usage);
 		static GLenum GetGLESPrimitiveType(PRIMITIVE_TYPE type);
		static GLenum GetGLESIndexType(INDEX_TYPE type);
		static void	  GetGLESDeclType(DECL_USAGE Usage, GLenum& type, 
			GLint& typeCount,GLboolean& normalized,std::string& name);
		static void	  GetGLESFilter(FilterOptions type, GLenum& minFilter, GLenum& magFilter);
		static GLenum GetGLESWrap(Wrap type);
// 		//static CGprofile    GetCgProfile(SHADER_PROFILE Profile);
// 
// 		static std::string   GetHLSLProfile(SHADER_PROFILE Profile);
// 
// 		static int          GetTypeSize(SHADER_PARAM_TYPE Type);
// 
// 		static D3DXIMAGE_FILEFORMAT GetD3DXImageFileFormat(IMAGE_FILE_FORMAT Format);
// 
// 		static std::string   GetD3DErrorDescription(HRESULT hr);
// 		static std::string   GetD3DErrorString(HRESULT hr);
// 
// 		static USAGE        GetUsage(DWORD Usage);
// 		static int          GetLock(DWORD Lock);
// 		static FORMAT       GetFormat(D3DFORMAT D3DFormat);
// 		static TEXTURE_FILTER_TYPE GetFilterType(DWORD Type);
// 		static TEXTURE_ADDRESS_MODE GetAddressMode(DWORD Mode);
// 		static DECL_USAGE   GetDeclUsage(BYTE usage);
// 		static DECL_TYPE    GetDeclType(BYTE type);
// 
// 		static D3DMULTISAMPLE_TYPE GetD3DMultiSampleType(MULTI_SAMPLE ms);
// 		static MULTI_SAMPLE GetMultiSampleType(D3DMULTISAMPLE_TYPE ms);
// 
// 		static std::string   GetMSAAString(MULTI_SAMPLE msaa);
// 		static std::string   GetUsageString(USAGE usage);
// 		static std::string   GetFormatString(FORMAT format);
// 		static std::string   GetShaderProfileString(SHADER_PROFILE profile);
	};

} 

#endif
