#pragma once

#import <Metal/Metal.h>

namespace ma
{

	class MetalMapping
	{
	public:
		//static DXGI_FORMAT	GetD3DIndexType(INDEX_TYPE eType);
        static MTLPixelFormat  _getPF(PixelFormat Format);
		static void GetFilter(Filter type,MTLSamplerMinMagFilter& minmag,MTLSamplerMipFilter& mipmap);
		static MTLSamplerAddressMode   GetWrap(Wrap type);
		static const char*  GetDeclUsage(DECL_USAGE DeclUsage);
		static MTLVertexFormat  GetDeclType(DECL_TYPE DeclType);
		static MTLPrimitiveType GetPrimitiveType(PRIMITIVE_TYPE type);

		static MTLCompareFunction get(CompareFunction cf);

		static MTLStencilOperation get(StencilOperation op);

		static MTLTriangleFillMode get(FillMode mode);

		static MTLCullMode get(CULL_MODE mode);

		static void GetBlend(BLEND_MODE mode,bool& bEnbale,MTLBlendFactor& src,MTLBlendFactor& destm,MTLBlendOperation& op);
		static void GetBlendAlpha(BLEND_MODE mode,bool& bEnbale,MTLBlendFactor& src,MTLBlendFactor& destm,MTLBlendOperation& op);

		static PixelFormat _getClosestSupportedPF(PixelFormat ogrePF);
	};

} 


