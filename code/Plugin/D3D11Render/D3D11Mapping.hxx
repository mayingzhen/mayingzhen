#include "D3D11Mapping.h"

namespace ma
{

// D3D11_USAGE D3D11Mapping::GetD3DTextureUsage(TEXTURE_USAGE Usage)
// {
//     DWORD D3D11Usage = 0;
// 
// 	if (Usage == D3DUSAGE_RENDERTARGET)
// 		D3D11Usage = USAGE_RENDERTARGET;
// 	else if (Usage == USAGE_DEPTHSTENCIL)
// 		D3D11Usage = D3DUSAGE_DEPTHSTENCIL;
//     else if (Usage == USAGE_DYNAMIC)
//         D3D11Usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
// 
//     return D3D11Usage;
// }

// D3D11_USAGE D3D11Mapping::GetD3DBufferUsage(HBU_USAGE Usage)
// {
// 	uint32 ret = 0;
// 	if (Usage & HBU_DYNAMIC)
// 	{
// 		// Only add the dynamic flag for default pool, and
// 		// we use default pool when buffer is discardable
// 		if (Usage & HBU_DISCARDABLE)
// 			ret |= D3DUSAGE_DYNAMIC;
// 	}
// 
// 	if (Usage & HBU_WRITE_ONLY)
// 	{
// 		ret |= D3DUSAGE_WRITEONLY;
// 	}
// 	return ret;
// }

// D3DPOOL D3D11Mapping::GetD3DPool(HBU_USAGE Usage)
// {
//     D3DPOOL D3DPool = D3DPOOL_MANAGED;
// 
//     if (Usage & HBU_DISCARDABLE)
//         D3DPool = D3DPOOL_DEFAULT;
// 
//     return D3DPool;
// }

// DWORD D3D11Mapping::GetD3DLock(int Lock)
// {
//     DWORD D3DLock = 0;
// 
//     if (Lock & LOCK_DISCARD)
//         D3DLock |= D3DLOCK_DISCARD;
//     if (Lock & LOCK_NOOVERWRITE)
//         D3DLock |= D3DLOCK_NOOVERWRITE;
//     if (Lock & LOCK_READONLY)
//         D3DLock |= D3DLOCK_READONLY;
// 
//     return D3DLock;
// }

DXGI_FORMAT D3D11Mapping::GetD3DIndexType(INDEX_TYPE eType)
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

PixelFormat D3D11Mapping::_getClosestSupportedPF(PixelFormat ogrePF)
{
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
}

DXGI_FORMAT D3D11Mapping::_getPF(PixelFormat ogrePF)
{
	switch(ogrePF)
	{
	case PF_L8:
		return DXGI_FORMAT_R8_UNORM;
	case PF_L16:
		return DXGI_FORMAT_R16_UNORM;
	case PF_A8:
		return DXGI_FORMAT_UNKNOWN;
	case PF_A4L4:
		return DXGI_FORMAT_UNKNOWN;
	case PF_BYTE_LA:
		return DXGI_FORMAT_UNKNOWN; 
	case PF_R3G3B2:
		return DXGI_FORMAT_UNKNOWN;
	case PF_A1R5G5B5:
		return DXGI_FORMAT_UNKNOWN;
	case PF_R5G6B5:
		return DXGI_FORMAT_UNKNOWN;
	case PF_A4R4G4B4:
		return DXGI_FORMAT_UNKNOWN;
	case PF_R8G8B8:
		return DXGI_FORMAT_UNKNOWN;
	case PF_A8R8G8B8:
		return DXGI_FORMAT_UNKNOWN;
	case PF_A8B8G8R8:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case PF_X8R8G8B8:
		return DXGI_FORMAT_UNKNOWN;
	case PF_X8B8G8R8:
		return DXGI_FORMAT_UNKNOWN;
	case PF_A2B10G10R10:
		return DXGI_FORMAT_R10G10B10A2_TYPELESS;
	case PF_A2R10G10B10:
		return DXGI_FORMAT_UNKNOWN;
	case PF_FLOAT16_R:
		return DXGI_FORMAT_R16_FLOAT;
	case PF_FLOAT16_RGBA:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case PF_FLOAT32_R:
		return DXGI_FORMAT_R32_FLOAT;
	case PF_FLOAT32_RGBA:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case PF_SHORT_RGBA:
		return DXGI_FORMAT_R16G16B16A16_UNORM;
	case PF_DXT1:
		return DXGI_FORMAT_BC1_UNORM;
	case PF_DXT2:
		return DXGI_FORMAT_BC2_UNORM;
	case PF_DXT3:
		return DXGI_FORMAT_BC3_UNORM;
	case PF_DXT4:
		return DXGI_FORMAT_BC4_UNORM;
	case PF_DXT5:
		return DXGI_FORMAT_BC5_UNORM;
	case PF_UNKNOWN:
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}


DXGI_FORMAT D3D11Mapping::GetD3DDeclType(DECL_TYPE DeclType)
{
    switch (DeclType)
    {
    case DT_FLOAT1:
        return DXGI_FORMAT_R32_FLOAT;

    case DT_FLOAT2:
        return DXGI_FORMAT_R32G32_FLOAT;

    case DT_FLOAT3:
        return DXGI_FORMAT_R32G32B32_FLOAT;

    case DT_FLOAT4:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;

    case DT_COLOR:
        return DXGI_FORMAT_R8G8B8A8_SNORM;

    case DT_UBYTE4:
		return DXGI_FORMAT_R8G8B8A8_UINT;

	case  DT_UBYTE4N:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

    case DT_SHORT2:
		return DXGI_FORMAT_R16G16_SINT;

	case DT_SHORT2N:
		return DXGI_FORMAT_R16G16_SNORM;

    case DT_SHORT4:
		return DXGI_FORMAT_R16G16B16A16_SINT;

	case DT_SHORT4N:
		return DXGI_FORMAT_R16G16B16A16_SNORM;

    default:
        ASSERT(0 && "Invalid Declaration Type.");
        return DXGI_FORMAT_UNKNOWN;
    }
}

// int D3D11Mapping::GetD3DDeclTypeSize(BYTE type)
// {
//     switch (type)
//     {
//     case D3DDECLTYPE_FLOAT1:
//         return 4;
// 
//     case D3DDECLTYPE_FLOAT2:
//         return 8;
// 
//     case D3DDECLTYPE_FLOAT3:
//         return 12;
// 
//     case D3DDECLTYPE_FLOAT4:
//         return 16;
// 
//     case D3DDECLTYPE_D3DCOLOR:
//         return 4;
// 
//     case D3DDECLTYPE_UBYTE4:
//         return 4;
// 
//     case D3DDECLTYPE_SHORT2:
//         return 4;
// 
//     case D3DDECLTYPE_SHORT4:
//         return 8;
// 
//     case D3DDECLTYPE_UBYTE4N:
//         return 4;
// 
//     case D3DDECLTYPE_SHORT2N:
//         return 4;
// 
//     case D3DDECLTYPE_SHORT4N:
//         return 8;
// 
//     case D3DDECLTYPE_USHORT2N:
//         return 4;
// 
//     case D3DDECLTYPE_USHORT4N:
//         return 8;
// 
//     case D3DDECLTYPE_FLOAT16_2:
//         return 4;
// 
//     case D3DDECLTYPE_FLOAT16_4:
//         return 8;
// 
//     default:
//         ASSERT(0 && "Invalid Declaration Type.");
//         return 0;
//     }
// }


D3D_PRIMITIVE_TOPOLOGY D3D11Mapping::GetD3DPrimitiveType(PRIMITIVE_TYPE type)
{
	switch (type)
	{
	case PRIM_POINTLIST:
		return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	case PRIM_LINELIST:
		return D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	case PRIM_LINESTRIP:
		return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

	case PRIM_TRIANGLELIST:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	case PRIM_TRIANGLESTRIP:
		return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

	case PRIM_TRIANGLEFAN:
		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

	default:
		ASSERT(0 && "Invalid Declaration Usage.");
		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	} 
}


const char* D3D11Mapping::GetD3DDeclUsage(DECL_USAGE DeclUsage)
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
    }

	//return D3DDECLUSAGE_POSITION;
}


D3D11_FILTER D3D11Mapping::GetD3D11Filter(Filter type)
{
	if (type == TFO_POINT)
	{
		return  D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
	else if (type == TFO_BILINEAR)
	{
		return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	}
	else if (type == TFO_TRILINEAR)
	{
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
	else if (type == TFO_ANISOTROPIC)
	{
		return D3D11_FILTER_ANISOTROPIC;
	}
	else
	{
		ASSERT(false);
	}
}


D3D11_TEXTURE_ADDRESS_MODE		D3D11Mapping::GetD3D11Wrap(Wrap type)
{
	if (type == REPEAT)
	{
		return D3D11_TEXTURE_ADDRESS_WRAP;
	}
	else if (type == CLAMP)
	{
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	}
	else 
	{
		ASSERT(false);
	}

	return D3D11_TEXTURE_ADDRESS_WRAP;
}

D3D11_COMPARISON_FUNC D3D11Mapping::get(CompareFunction cf)
{
	switch( cf )
	{
	case CMPF_ALWAYS_FAIL:
		return D3D11_COMPARISON_NEVER;
	case CMPF_ALWAYS_PASS:
		return D3D11_COMPARISON_ALWAYS;
	case CMPF_LESS:
		return D3D11_COMPARISON_LESS;
	case CMPF_LESS_EQUAL:
		return D3D11_COMPARISON_LESS_EQUAL;
	case CMPF_EQUAL:
		return D3D11_COMPARISON_EQUAL;
	case CMPF_NOT_EQUAL:
		return D3D11_COMPARISON_NOT_EQUAL;
	case CMPF_GREATER_EQUAL:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case CMPF_GREATER:
		return D3D11_COMPARISON_GREATER;
	};
}

D3D11_STENCIL_OP D3D11Mapping::get(StencilOperation op, bool invert)
{
	switch(op)
	{
	case SOP_KEEP:
		return D3D11_STENCIL_OP_KEEP;
	case SOP_ZERO:
		return D3D11_STENCIL_OP_ZERO;
	case SOP_REPLACE:
		return D3D11_STENCIL_OP_REPLACE;
	case SOP_INCREMENT:
		return invert? D3D11_STENCIL_OP_DECR_SAT : D3D11_STENCIL_OP_INCR_SAT;
	case SOP_DECREMENT:
		return invert? D3D11_STENCIL_OP_INCR_SAT : D3D11_STENCIL_OP_DECR_SAT;
	case SOP_INCREMENT_WRAP:
		return invert? D3D11_STENCIL_OP_DECR : D3D11_STENCIL_OP_INCR;
	case SOP_DECREMENT_WRAP:
		return invert? D3D11_STENCIL_OP_INCR : D3D11_STENCIL_OP_DECR;
	case SOP_INVERT:
		return D3D11_STENCIL_OP_INVERT;
	}
}

void D3D11Mapping::GetD3DBlend(BLEND_MODE mode,BOOL& bEnbale,D3D11_BLEND& src,D3D11_BLEND& dest,D3D11_BLEND_OP& op)
{
	switch (mode)
	{
	case BM_OPATICY:
		bEnbale = FALSE;
		break;

	case BM_TRANSPARENT:
		bEnbale = TRUE;
		src = D3D11_BLEND_SRC_ALPHA;
		dest = D3D11_BLEND_INV_SRC_ALPHA;
		op = D3D11_BLEND_OP_ADD;
		break;

	case BM_ADD:
		bEnbale = TRUE;
		src = D3D11_BLEND_ONE;
		dest = D3D11_BLEND_ONE;
		op = D3D11_BLEND_OP_ADD;
		break;

	case BM_MULTIPLY:
		bEnbale = TRUE;
		src = D3D11_BLEND_DEST_COLOR;
		dest = D3D11_BLEND_ZERO;
		op = D3D11_BLEND_OP_ADD;
		break;
	}
}

D3D11_FILL_MODE D3D11Mapping::get(FillMode mode)
{
	if (mode == FILL_SOLID)
	{
		return D3D11_FILL_SOLID;
	}
	else if (mode == FILL_WIREFRAME)
	{
		return D3D11_FILL_WIREFRAME;
	}
	else if (mode == FILL_POINT)
	{
		return D3D11_FILL_WIREFRAME;
	}
}


D3D11_CULL_MODE D3D11Mapping::get(CULL_MODE mode)
{
	if (mode == CULL_FACE_SIDE_NONE)
	{
		return D3D11_CULL_NONE;
	}
	else if (mode == CULL_FACE_SIDE_BACK)
	{
		return D3D11_CULL_BACK;
	}
	else if (mode == CULL_FACE_SIDE_FRONT)
	{
		return D3D11_CULL_FRONT;
	}
}

//#define  _D3D_V(str) str
void D3D11Verify( HRESULT hr )
{
	//#if defined(_DEBUG)
	if( FAILED( hr ) ) 
	{
		ASSERT(false);
		//LogError("D3D11Verify:%d, %s, %s", hr, DXGetErrorString( hr ), DXGetErrorDescription( hr ));
	}
	//#endif
}

void D3D11Verify( HRESULT hr, const char *info ) 
{
	//#if defined(_DEBUG)
	if( FAILED( hr ) ) 
	{
		ASSERT(false);
		//LogError("D3D11Verify:%d, %s, %s, %s", hr, DXGetErrorString( hr ), DXGetErrorDescription( hr ), info);
	}
	//#endif
}

}