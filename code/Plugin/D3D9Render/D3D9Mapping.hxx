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

uint32 D3D9Mapping::get(CompareFunction cf)
{
	switch( cf )
	{
	case CMPF_ALWAYS_FAIL:
		return D3DCMP_NEVER;
	case CMPF_ALWAYS_PASS:
		return D3DCMP_ALWAYS;
	case CMPF_LESS:
		return D3DCMP_LESS;
	case CMPF_LESS_EQUAL:
		return D3DCMP_LESSEQUAL;
	case CMPF_EQUAL:
		return D3DCMP_EQUAL;
	case CMPF_NOT_EQUAL:
		return D3DCMP_NOTEQUAL;
	case CMPF_GREATER_EQUAL:
		return D3DCMP_GREATEREQUAL;
	case CMPF_GREATER:
		return D3DCMP_GREATER;
	};
	return 0;
}

uint32 D3D9Mapping::get(StencilOperation op, bool invert)
{
	switch(op)
	{
	case SOP_KEEP:
		return D3DSTENCILOP_KEEP;
	case SOP_ZERO:
		return D3DSTENCILOP_ZERO;
	case SOP_REPLACE:
		return D3DSTENCILOP_REPLACE;
	case SOP_INCREMENT:
		return invert? D3DSTENCILOP_DECRSAT : D3DSTENCILOP_INCRSAT;
	case SOP_DECREMENT:
		return invert? D3DSTENCILOP_INCRSAT : D3DSTENCILOP_DECRSAT;
	case SOP_INCREMENT_WRAP:
		return invert? D3DSTENCILOP_DECR : D3DSTENCILOP_INCR;
	case SOP_DECREMENT_WRAP:
		return invert? D3DSTENCILOP_INCR : D3DSTENCILOP_DECR;
	case SOP_INVERT:
		return D3DSTENCILOP_INVERT;
	}
	return 0;
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
