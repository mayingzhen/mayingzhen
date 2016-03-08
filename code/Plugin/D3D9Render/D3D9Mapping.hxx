#include "D3D9Mapping.h"

namespace ma
{

DWORD D3D9Mapping::GetD3DTextureUsage(TEXTURE_USAGE Usage)
{
    DWORD D3D9Usage = 0;

	if (Usage == D3DUSAGE_RENDERTARGET)
		D3D9Usage = D3DUSAGE_RENDERTARGET;
	else if (Usage == USAGE_DEPTHSTENCIL)
		D3D9Usage = D3DUSAGE_DEPTHSTENCIL;
    else if (Usage == USAGE_DYNAMIC)
        D3D9Usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;

    return D3D9Usage;
}

DWORD D3D9Mapping::GetD3DBufferUsage(HBU_USAGE Usage)
{
	uint32 ret = 0;
	if (Usage & HBU_DYNAMIC)
	{
		// Only add the dynamic flag for default pool, and
		// we use default pool when buffer is discardable
		if (Usage & HBU_DISCARDABLE)
			ret |= D3DUSAGE_DYNAMIC;
	}

	if (Usage & HBU_WRITE_ONLY)
	{
		ret |= D3DUSAGE_WRITEONLY;
	}
	return ret;
}

D3DPOOL D3D9Mapping::GetD3DPool(HBU_USAGE Usage)
{
    D3DPOOL D3DPool = D3DPOOL_MANAGED;

    if (Usage & HBU_DISCARDABLE)
        D3DPool = D3DPOOL_DEFAULT;

    return D3DPool;
}

DWORD D3D9Mapping::GetD3DLock(int Lock)
{
    DWORD D3DLock = 0;

    if (Lock & LOCK_DISCARD)
        D3DLock |= D3DLOCK_DISCARD;
    if (Lock & LOCK_NOOVERWRITE)
        D3DLock |= D3DLOCK_NOOVERWRITE;
    if (Lock & LOCK_READONLY)
        D3DLock |= D3DLOCK_READONLY;

    return D3DLock;
}

D3DFORMAT D3D9Mapping::GetD3DIndexType(INDEX_TYPE eType)
{
	if (eType == INDEX_TYPE_U16)
	{
		return D3DFMT_INDEX16;
	}
	else
	{
		return D3DFMT_INDEX32;
	}
}

D3DFORMAT D3D9Mapping::GetD3DFormat(PixelFormat ogrePF)
{
	switch(ogrePF)
	{
	case PF_L8:
		return D3DFMT_L8;
	case PF_L16:
		return D3DFMT_L16;
	case PF_A8:
		return D3DFMT_A8;
	case PF_A4L4:
		return D3DFMT_A4L4;
	case PF_BYTE_LA:
		return D3DFMT_A8L8; // Assume little endian here
	case PF_R3G3B2:
		return D3DFMT_R3G3B2;
	case PF_A1R5G5B5:
		return D3DFMT_A1R5G5B5;
	case PF_R5G6B5:
		return D3DFMT_R5G6B5;
	case PF_A4R4G4B4:
		return D3DFMT_A4R4G4B4;
	case PF_R8G8B8:
		return D3DFMT_R8G8B8;
	case PF_A8R8G8B8:
		return D3DFMT_A8R8G8B8;
	case PF_A8B8G8R8:
		return D3DFMT_A8B8G8R8;
	case PF_X8R8G8B8:
		return D3DFMT_X8R8G8B8;
	case PF_X8B8G8R8:
		return D3DFMT_X8B8G8R8;
	case PF_A2B10G10R10:
		return D3DFMT_A2B10G10R10;
	case PF_A2R10G10B10:
		return D3DFMT_A2R10G10B10;
	case PF_FLOAT16_R:
		return D3DFMT_R16F;
	case PF_FLOAT16_GR:
		return D3DFMT_G16R16F;
	case PF_FLOAT16_RGBA:
		return D3DFMT_A16B16G16R16F;
	case PF_FLOAT32_R:
		return D3DFMT_R32F;
	case PF_FLOAT32_GR:
		return D3DFMT_G32R32F;
	case PF_FLOAT32_RGBA:
		return D3DFMT_A32B32G32R32F;
	case PF_SHORT_RGBA:
		return D3DFMT_A16B16G16R16;
	case PF_SHORT_GR:
		return D3DFMT_G16R16;
	case PF_DXT1:
		return D3DFMT_DXT1;
	case PF_DXT2:
		return D3DFMT_DXT2;
	case PF_DXT3:
		return D3DFMT_DXT3;
	case PF_DXT4:
		return D3DFMT_DXT4;
	case PF_DXT5:
		return D3DFMT_DXT5;
	case PF_D24S8:
		return D3DFMT_D24S8;
	case PF_NULL:
		return (D3DFORMAT)MAKEFOURCC('N', 'U', 'L', 'L');
	case PF_INTZ:
		return (D3DFORMAT)MAKEFOURCC('I', 'N', 'T', 'Z'); 
	case PF_UNKNOWN:
	default:
		return D3DFMT_UNKNOWN;
	}
}

// D3DFORMAT D3D9Mapping::GetD3DFormat(FORMAT Format)
// {
//     switch (Format)
//     {
//     case FMT_UNKNOWN:
//         return D3DFMT_UNKNOWN;
// 
//     case FMT_R8G8B8:
//         return D3DFMT_R8G8B8;
// 
//     case FMT_A8R8G8B8:
//         return D3DFMT_A8R8G8B8;
// 
//     case FMT_X8R8G8B8:
//         return D3DFMT_X8R8G8B8;
// 
//     case FMT_R5G6B5:
//         return D3DFMT_R5G6B5;
// 
//     case FMT_X1R5G5B5:
//         return D3DFMT_X1R5G5B5;
// 
//     case FMT_A1R5G5B5:
//         return D3DFMT_A1R5G5B5;
// 
//     case FMT_A4R4G4B4:
//         return D3DFMT_A4R4G4B4;
// 
//     case FMT_R3G3B2:
//         return D3DFMT_R3G3B2;
// 
//     case FMT_A8:
//         return D3DFMT_A8;
// 
//     case FMT_A8R3G3B2:
//         return D3DFMT_A8R3G3B2;
// 
//     case FMT_X4R4G4B4:
//         return D3DFMT_X4R4G4B4;
// 
//     case FMT_A2B10G10R10:
//         return D3DFMT_A2B10G10R10;
// 
//     case FMT_A8B8G8R8:
//         return D3DFMT_A8B8G8R8;
// 
//     case FMT_X8B8G8R8:
//         return D3DFMT_X8B8G8R8;
// 
//     case FMT_G16R16:
//         return D3DFMT_G16R16;
// 
//     case FMT_A2R10G10B10:
//         return D3DFMT_A2R10G10B10;
// 
//     case FMT_A16B16G16R16:
//         return D3DFMT_A16B16G16R16;
// 
//     case FMT_A8P8:
//         return D3DFMT_A8P8;
// 
//     case FMT_P8:
//         return D3DFMT_P8;
// 
//     case FMT_L8:
//         return D3DFMT_L8;
// 
//     case FMT_A8L8:
//         return D3DFMT_A8L8;
// 
//     case FMT_A4L4:
//         return D3DFMT_A4L4;
// 
//     case FMT_V8U8:
//         return D3DFMT_V8U8;
// 
//     case FMT_L6V5U5:
//         return D3DFMT_L6V5U5;
// 
//     case FMT_X8L8V8U8:
//         return D3DFMT_X8L8V8U8;
// 
//     case FMT_Q8W8V8U8:
//         return D3DFMT_Q8W8V8U8;
// 
//     case FMT_V16U16:
//         return D3DFMT_V16U16;
// 
//     case FMT_A2W10V10U10:
//         return D3DFMT_A2W10V10U10;
// 
//     case FMT_UYVY:
//         return D3DFMT_UYVY;
// 
//     case FMT_R8G8_B8G8:
//         return D3DFMT_R8G8_B8G8;
// 
//     case FMT_YUY2:
//         return D3DFMT_YUY2;
// 
//     case FMT_G8R8_G8B8:
//         return D3DFMT_G8R8_G8B8;
// 
//     case FMT_DXT1:
//         return D3DFMT_DXT1;
// 
//     case FMT_DXT2:
//         return D3DFMT_DXT2;
// 
//     case FMT_DXT3:
//         return D3DFMT_DXT3;
// 
//     case FMT_DXT4:
//         return D3DFMT_DXT4;
// 
//     case FMT_DXT5:
//         return D3DFMT_DXT5;
// 
//     case FMT_D16_LOCKABLE:
//         return D3DFMT_D16_LOCKABLE;
// 
//     case FMT_D32:
//         return D3DFMT_D32;
// 
//     case FMT_D15S1:
//         return D3DFMT_D15S1;
// 
//     case FMT_D24S8:
//         return D3DFMT_D24S8;
// 
//     case FMT_D24X8:
//         return D3DFMT_D24X8;
// 
//     case FMT_D24X4S4:
//         return D3DFMT_D24X4S4;
// 
//     case FMT_D16:
//         return D3DFMT_D16;
// 
//     case FMT_D32F_LOCKABLE:
//         return D3DFMT_D32F_LOCKABLE;
// 
//     case FMT_D24FS8:
//         return D3DFMT_D24FS8;
// 
//     case FMT_L16:
//         return D3DFMT_L16;
// 
//     case FMT_VERTEXDATA:
//         return D3DFMT_VERTEXDATA;
// 
//     case FMT_INDEX16:
//         return D3DFMT_INDEX16;
// 
//     case FMT_INDEX32:
//         return D3DFMT_INDEX32;
// 
//     case FMT_Q16W16V16U16:
//         return D3DFMT_Q16W16V16U16;
// 
//     case FMT_MULTI2_ARGB8:
//         return D3DFMT_MULTI2_ARGB8;
// 
//     case FMT_R16F:
//         return D3DFMT_R16F;
// 
//     case FMT_G16R16F:
//         return D3DFMT_G16R16F;
// 
//     case FMT_A16B16G16R16F:
//         return D3DFMT_A16B16G16R16F;
// 
//     case FMT_R32F:
//         return D3DFMT_R32F;
// 
//     case FMT_G32R32F:
//         return D3DFMT_G32R32F;
// 
//     case FMT_A32B32G32R32F:
//         return D3DFMT_A32B32G32R32F;
// 
//     case FMT_D32_LOCKABLE:
//         return D3DFMT_D32_LOCKABLE;
// 
//     case FMT_S8_LOCKABLE:
//         return D3DFMT_S8_LOCKABLE;
// 
//     case FMT_CxV8U8:
//         return D3DFMT_CxV8U8;
// 
//     case FMT_A1:
//         return D3DFMT_A1;
// 
//     case FMT_BINARYBUFFER:
//         return D3DFMT_BINARYBUFFER;
// 
//     default:
//         ASSERT(0 && "Invalid Format.");
//         return D3DFMT_UNKNOWN;
//     }
// }

BYTE D3D9Mapping::GetD3DDeclType(DECL_TYPE DeclType)
{
    switch (DeclType)
    {
    case DT_FLOAT1:
        return D3DDECLTYPE_FLOAT1;

    case DT_FLOAT2:
        return D3DDECLTYPE_FLOAT2;

    case DT_FLOAT3:
        return D3DDECLTYPE_FLOAT3;

    case DT_FLOAT4:
        return D3DDECLTYPE_FLOAT4;

    case DT_COLOR:
        return D3DDECLTYPE_D3DCOLOR;

    case DT_UBYTE4:
        return D3DDECLTYPE_UBYTE4;

	case  DT_UBYTE4N:
		return D3DDECLTYPE_UBYTE4N;

    case DT_SHORT2:
        return D3DDECLTYPE_SHORT2;

    case DT_SHORT4:
        return D3DDECLTYPE_SHORT4;

    default:
        ASSERT(0 && "Invalid Declaration Type.");
        return 0;
    }
}

int D3D9Mapping::GetD3DDeclTypeSize(BYTE type)
{
    switch (type)
    {
    case D3DDECLTYPE_FLOAT1:
        return 4;

    case D3DDECLTYPE_FLOAT2:
        return 8;

    case D3DDECLTYPE_FLOAT3:
        return 12;

    case D3DDECLTYPE_FLOAT4:
        return 16;

    case D3DDECLTYPE_D3DCOLOR:
        return 4;

    case D3DDECLTYPE_UBYTE4:
        return 4;

    case D3DDECLTYPE_SHORT2:
        return 4;

    case D3DDECLTYPE_SHORT4:
        return 8;

    case D3DDECLTYPE_UBYTE4N:
        return 4;

    case D3DDECLTYPE_SHORT2N:
        return 4;

    case D3DDECLTYPE_SHORT4N:
        return 8;

    case D3DDECLTYPE_USHORT2N:
        return 4;

    case D3DDECLTYPE_USHORT4N:
        return 8;

    case D3DDECLTYPE_FLOAT16_2:
        return 4;

    case D3DDECLTYPE_FLOAT16_4:
        return 8;

    default:
        ASSERT(0 && "Invalid Declaration Type.");
        return 0;
    }
}


D3DPRIMITIVETYPE D3D9Mapping::GetD3DPrimitiveType(PRIMITIVE_TYPE type)
{
	switch (type)
	{
	case PRIM_POINTLIST:
		return D3DPT_POINTLIST;

	case PRIM_LINELIST:
		return D3DPT_LINELIST;

	case PRIM_LINESTRIP:
		return D3DPT_LINESTRIP;

	case PRIM_TRIANGLELIST:
		return D3DPT_TRIANGLELIST;

	case PRIM_TRIANGLESTRIP:
		return D3DPT_TRIANGLESTRIP;

	case PRIM_TRIANGLEFAN:
		return D3DPT_TRIANGLEFAN;

	default:
		ASSERT(0 && "Invalid Declaration Usage.");
		return D3DPT_TRIANGLELIST;
	} 
}


D3DDECLUSAGE D3D9Mapping::GetD3DDeclUsage(DECL_USAGE DeclUsage)
{
    switch (DeclUsage)
    {
    case DU_POSITION:
        return D3DDECLUSAGE_POSITION;

    case DU_BLENDWEIGHT:
        return D3DDECLUSAGE_BLENDWEIGHT;

    case DU_BLENDINDICES:
        return D3DDECLUSAGE_BLENDINDICES;

    case DU_NORMAL:
        return D3DDECLUSAGE_NORMAL;

    case DU_TEXCOORD:
        return D3DDECLUSAGE_TEXCOORD;

    case DU_TANGENT:
        return D3DDECLUSAGE_TANGENT;

    case DU_COLOR:
        return D3DDECLUSAGE_COLOR;

    default:
        ASSERT(0 && "Invalid Declaration Usage.");
    }

	return D3DDECLUSAGE_POSITION;
}


void D3D9Mapping::GetD3D9Filter(Filter type,DWORD& minFilter,DWORD& magFilter,DWORD& mipFilter)
{
	if (type == TFO_POINT)
	{
		minFilter = D3DTEXF_POINT;
		magFilter = D3DTEXF_POINT;
		mipFilter = D3DTEXF_NONE;
	}
	else if (type == TFO_BILINEAR)
	{
		minFilter = D3DTEXF_LINEAR;
		magFilter = D3DTEXF_LINEAR;
		mipFilter = D3DTEXF_POINT;
	}
	else if (type == TFO_TRILINEAR)
	{
		minFilter = D3DTEXF_LINEAR;
		magFilter = D3DTEXF_LINEAR;
		mipFilter = D3DTEXF_LINEAR;
	}
	else if (type == TFO_ANISOTROPIC)
	{
		minFilter = D3DTEXF_ANISOTROPIC;
		magFilter = D3DTEXF_ANISOTROPIC;
		mipFilter = D3DTEXF_LINEAR;
	}
	else
	{
		minFilter = D3DTEXF_POINT;
		magFilter = D3DTEXF_POINT;
		mipFilter = D3DTEXF_NONE;
		ASSERT(false);
	}
}


DWORD		D3D9Mapping::GetD3D9Wrap(Wrap type)
{
	if (type == REPEAT)
	{
		return D3DTADDRESS_WRAP;
	}
	else if (type == CLAMP)
	{
		return D3DTADDRESS_CLAMP;
	}
	else 
	{
		ASSERT(false);
	}

	return D3DTADDRESS_WRAP;
}


//#define  _D3D_V(str) str
void D3D9Verify( HRESULT hr )
{
	//#if defined(_DEBUG)
	if( FAILED( hr ) ) 
	{
		ASSERT(false);
		//LogError("D3D9Verify:%d, %s, %s", hr, DXGetErrorString( hr ), DXGetErrorDescription( hr ));
	}
	//#endif
}

void D3D9Verify( HRESULT hr, const char *info ) 
{
	//#if defined(_DEBUG)
	if( FAILED( hr ) ) 
	{
		ASSERT(false);
		//LogError("D3D9Verify:%d, %s, %s, %s", hr, DXGetErrorString( hr ), DXGetErrorDescription( hr ), info);
	}
	//#endif
}

}
