#include "MetalMapping.h"

namespace ma
{
/*
DXGI_FORMAT MetalMapping::GetD3DIndexType(INDEX_TYPE eType)
{
	if (eType == INDEX_TYPE_U16)
	{
		return DXGI_FORMAT_R16_UINT;
	}
	else
	{
		return DXGI_FORMAT_R32_UINT;
	}
}
*/
    
PixelFormat MetalMapping::_getClosestSupportedPF(PixelFormat ogrePF)
{
    return ogrePF;
    /*
	if (_getPF(ogrePF) != DXGI_FORMAT_UNKNOWN)
	{
		return ogrePF;
	}
	switch(ogrePF)
	{
	case PF_FLOAT16_RGB:
		return PF_FLOAT16_RGBA;
	case PF_FLOAT32_RGB:
		return PF_FLOAT32_RGBA;
	case PF_UNKNOWN:
	default:
		return PF_A8B8G8R8;
	}
     */
}


MTLPixelFormat MetalMapping::_getPF(PixelFormat ogrePF)
{
	switch(ogrePF)
	{
	//case PF_L8:
	//	return MTLPixelFormatR8Uint;
	//case PF_L16:
	//	return MTLPixelFormatR16Uint;
	//case PF_A8:
	//	return MTLPixelFormatA8Unorm;
	//case PF_A4L4:
	//	return DXGI_FORMAT_UNKNOWN;
	//case PF_BYTE_LA:
	//	return DXGI_FORMAT_UNKNOWN;
	case PF_R3G3B2:
		return MTLPixelFormatRGBA32Float;
	//case PF_A1R5G5B5:
	//	return DXGI_FORMAT_UNKNOWN;
	//case PF_R5G6B5:
	//	return DXGI_FORMAT_UNKNOWN;
	//case PF_A4R4G4B4:
	//	return DXGI_FORMAT_UNKNOWN;
	//case PF_R8G8B8:
	//	return DXGI_FORMAT_UNKNOWN;
	case PF_A8R8G8B8:
		return MTLPixelFormatRGBA8Unorm;
	case PF_A8B8G8R8:
		return MTLPixelFormatBGRA8Unorm;
	//case PF_X8R8G8B8:
	//	return MTLPixelFormatRGBA8Unorm;
	//case PF_X8B8G8R8:
	//	return DXGI_FORMAT_UNKNOWN;
	//case PF_A2B10G10R10:
	//	return MTLPixelFormatRGB10A2Unorm;
	case PF_A2R10G10B10:
		return MTLPixelFormatRGB10A2Unorm;
	case PF_FLOAT16_R:
		return MTLPixelFormatR16Float;
	case PF_FLOAT16_RGBA:
		return MTLPixelFormatRGBA16Float;
	case PF_FLOAT32_R:
		return MTLPixelFormatR32Float;
	case PF_FLOAT32_RGBA:
		return MTLPixelFormatRGBA32Float;
	//case PF_SHORT_RGBA:
	//	return MTLPixelFormatRGBA16Uint;
	//case PF_DXT1:
	//	return MTLPixelFormatBC1_RGBA;
	//case PF_DXT2:
	//	return DXGI_FORMAT_BC2_UNORM;
	//case PF_DXT3:
	//	return DXGI_FORMAT_BC2_UNORM;
	//case PF_DXT4:
	//	return DXGI_FORMAT_BC3_UNORM;
	//case PF_DXT5:
	//	return DXGI_FORMAT_BC3_UNORM;
	case PF_D24S8:
		return MTLPixelFormatDepth32Float_Stencil8;
	case PF_UNKNOWN:
	default:
		return MTLPixelFormatInvalid;
	}
}


MTLVertexFormat MetalMapping::GetDeclType(DECL_TYPE DeclType)
{
    switch (DeclType)
    {
    case DT_FLOAT1:
        return MTLVertexFormatFloat;

    case DT_FLOAT2:
       return MTLVertexFormatFloat2;

    case DT_FLOAT3:
        return MTLVertexFormatFloat3;

    case DT_FLOAT4:
        return MTLVertexFormatFloat4;

    case DT_COLOR:
        return MTLVertexFormatUChar4Normalized;

    case DT_UBYTE4:
		return MTLVertexFormatUChar4;

	case  DT_UBYTE4N:
		return MTLVertexFormatUChar4Normalized;

    case DT_SHORT2:
		return MTLVertexFormatShort2;

	case DT_SHORT2N:
		return MTLVertexFormatShort2Normalized;

    case DT_SHORT4:
		return MTLVertexFormatShort4;

	case DT_SHORT4N:
		return MTLVertexFormatUShort4Normalized;

    default:
        ASSERT(0 && "Invalid Declaration Type.");
        return MTLVertexFormatInvalid;
    }
}


MTLPrimitiveType MetalMapping::GetPrimitiveType(PRIMITIVE_TYPE type)
{
	switch (type)
	{
	case PRIM_POINTLIST:
		return MTLPrimitiveTypePoint;

	case PRIM_LINELIST:
		return MTLPrimitiveTypeLine;

	case PRIM_LINESTRIP:
		return MTLPrimitiveTypeLineStrip;

	case PRIM_TRIANGLELIST:
		return MTLPrimitiveTypeTriangle;

	case PRIM_TRIANGLESTRIP:
		return MTLPrimitiveTypeTriangleStrip;

	//case PRIM_TRIANGLEFAN:
	//	return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

	default:
		ASSERT(0 && "Invalid Declaration Usage.");
		return MTLPrimitiveTypeTriangle;
	} 
}


const char* MetalMapping::GetDeclUsage(DECL_USAGE DeclUsage)
{
    switch (DeclUsage)
    {
    case DU_POSITION:
        return "POSITION";

    case DU_BLENDWEIGHT:
        return "BLENDWEIGHT";

    case DU_BLENDINDICES:
        return "BLENDINDICES";

    case DU_NORMAL:
        return "NORMAL";

    case DU_TEXCOORD:
        return "TEXCOORD";

    case DU_TANGENT:
        return "TANGENT";

    case DU_COLOR:
        return "COLOR";

    default:
        ASSERT(0 && "Invalid Declaration Usage.");
		return "POSITION";
    }
}


void MetalMapping::GetFilter(Filter type,MTLSamplerMinMagFilter& minmag,MTLSamplerMipFilter& mipmap)
{
    
	if (type == TFO_POINT)
	{
        minmag = MTLSamplerMinMagFilterNearest;
        mipmap = MTLSamplerMipFilterNearest;
	}
	else if (type == TFO_BILINEAR)
	{
        minmag = MTLSamplerMinMagFilterNearest;
        mipmap = MTLSamplerMipFilterNearest;
	}
	else if (type == TFO_TRILINEAR)
	{
        minmag = MTLSamplerMinMagFilterNearest;
        mipmap = MTLSamplerMipFilterNearest;
	}
	else if (type == TFO_ANISOTROPIC)
	{
        minmag = MTLSamplerMinMagFilterNearest;
        mipmap = MTLSamplerMipFilterNearest;
	}
	else if (type == TFO_SHADOWCOMPARE)
	{
        minmag = MTLSamplerMinMagFilterNearest;
        mipmap = MTLSamplerMipFilterNearest;
	}
	else
	{
        minmag = MTLSamplerMinMagFilterNearest;
        mipmap = MTLSamplerMipFilterNearest;
		ASSERT(false);
	}
}


MTLSamplerAddressMode MetalMapping::GetWrap(Wrap type)
{
    switch(type)
    {
        case CLAMP:
            return MTLSamplerAddressModeClampToEdge;
        case REPEAT:
            return MTLSamplerAddressModeRepeat;
        default:
            ASSERT(false);
            return MTLSamplerAddressModeClampToEdge;
    }
}

MTLCompareFunction MetalMapping::get(CompareFunction cf)
{
	switch( cf )
	{
	case CMPF_ALWAYS_FAIL:
		return MTLCompareFunctionNever;
	case CMPF_ALWAYS_PASS:
		return MTLCompareFunctionAlways;
	case CMPF_LESS:
		return MTLCompareFunctionLess;
	case CMPF_LESS_EQUAL:
		return MTLCompareFunctionLessEqual;
	case CMPF_EQUAL:
		return MTLCompareFunctionEqual;
	case CMPF_NOT_EQUAL:
		return MTLCompareFunctionNotEqual;
	case CMPF_GREATER_EQUAL:
		return MTLCompareFunctionGreaterEqual;
	case CMPF_GREATER:
		return MTLCompareFunctionGreater;
	default:
		ASSERT(false);
		return MTLCompareFunctionAlways;
	};
}

MTLStencilOperation MetalMapping::get(StencilOperation op)
{
	switch(op)
	{
	case SOP_KEEP:
		return MTLStencilOperationKeep;
	case SOP_ZERO:
		return MTLStencilOperationZero;
	case SOP_REPLACE:
		return MTLStencilOperationReplace;
	case SOP_INCREMENT:
            return MTLStencilOperationIncrementClamp;
	case SOP_DECREMENT:
		return MTLStencilOperationDecrementClamp;
	case SOP_INCREMENT_WRAP:
            return MTLStencilOperationIncrementWrap;
	case SOP_DECREMENT_WRAP:
            return MTLStencilOperationDecrementWrap;
	case SOP_INVERT:
		return MTLStencilOperationInvert;
	default:
		ASSERT(false);
		return MTLStencilOperationKeep;
	}
}

void MetalMapping::GetBlend(BLEND_MODE mode,bool& bEnbale,MTLBlendFactor& src,MTLBlendFactor& dest,MTLBlendOperation& op)
{
	switch (mode)
	{
	case BM_OPATICY:
		bEnbale = false;
		break;

	case BM_TRANSPARENT:
		bEnbale = true;
        src = MTLBlendFactorSourceAlpha;
		dest = MTLBlendFactorOneMinusSourceAlpha;
		op = MTLBlendOperationAdd;
		break;

	case BM_ADD:
		bEnbale = true;
		src = MTLBlendFactorOne;
		dest = MTLBlendFactorOne;
		op = MTLBlendOperationAdd;
		break;

	case BM_MULTIPLY:
		bEnbale = true;
		src = MTLBlendFactorZero;
		dest = MTLBlendFactorSourceColor;
		op = MTLBlendOperationAdd;
		break;
	default:
		ASSERT(false);
		bEnbale = false;
	}
}

void MetalMapping::GetBlendAlpha(BLEND_MODE mode,bool& bEnbale,MTLBlendFactor& src,MTLBlendFactor& dest,MTLBlendOperation& op)
{
    switch (mode)
    {
        case BM_OPATICY:
            bEnbale = false;
            break;
            
        case BM_TRANSPARENT:
            bEnbale = true;
            src = MTLBlendFactorSourceAlpha;
            dest = MTLBlendFactorOneMinusSourceAlpha;
            op = MTLBlendOperationAdd;
            break;
            
        case BM_ADD:
            bEnbale = true;
            src = MTLBlendFactorOne;
            dest = MTLBlendFactorOne;
            op = MTLBlendOperationAdd;
            break;
            
        case BM_MULTIPLY:
            bEnbale = true;
            src = MTLBlendFactorZero;
            dest = MTLBlendFactorSourceColor;
            op = MTLBlendOperationAdd;
            break;
        default:
            ASSERT(false);
            bEnbale = false;
    }
}

MTLTriangleFillMode MetalMapping::get(FillMode mode)
{
	if (mode == FILL_SOLID)
	{
		return MTLTriangleFillModeFill;
	}
	else if (mode == FILL_WIREFRAME)
	{
		return MTLTriangleFillModeLines;
	}
	else if (mode == FILL_POINT)
	{
		return MTLTriangleFillModeLines;
	}
	else
	{
		ASSERT(FALSE);
		return MTLTriangleFillModeFill;
	}
}


MTLCullMode MetalMapping::get(CULL_MODE mode)
{
	if (mode == CULL_FACE_SIDE_NONE)
	{
		return MTLCullModeNone;
	}
	else if (mode == CULL_FACE_SIDE_BACK)
	{
		return MTLCullModeBack;
	}
	else if (mode == CULL_FACE_SIDE_FRONT)
	{
		return MTLCullModeFront;
	}
	else
	{
		ASSERT(FALSE);
		return MTLCullModeBack;
	}
}

}
