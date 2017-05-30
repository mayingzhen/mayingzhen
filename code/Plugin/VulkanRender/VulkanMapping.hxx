#include "VulkanMapping.h"

namespace ma
{

DXGI_FORMAT VulkanMapping::GetD3DIndexType(INDEX_TYPE eType)
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

PixelFormat VulkanMapping::_getClosestSupportedPF(PixelFormat ogrePF)
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

//https://msdn.microsoft.com/en-us/library/windows/desktop/hh308955(v=vs.85).aspx

DXGI_FORMAT VulkanMapping::_getPF(PixelFormat ogrePF)
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
		return DXGI_FORMAT_BC2_UNORM;
	case PF_DXT4:
		return DXGI_FORMAT_BC3_UNORM;
	case PF_DXT5:
		return DXGI_FORMAT_BC3_UNORM;
	case PF_D24S8:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case PF_UNKNOWN:
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}


DXGI_FORMAT VulkanMapping::GetD3DDeclType(DECL_TYPE DeclType)
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
        return DXGI_FORMAT_R8G8B8A8_UNORM;

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


D3D_PRIMITIVE_TOPOLOGY VulkanMapping::GetD3DPrimitiveType(PRIMITIVE_TYPE type)
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
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	case PRIM_TRIANGLEFAN:
		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

	default:
		ASSERT(0 && "Invalid Declaration Usage.");
		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	} 
}


const char* VulkanMapping::GetD3DDeclUsage(DECL_USAGE DeclUsage)
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


Vulkan_FILTER VulkanMapping::GetVulkanFilter(Filter type)
{
	if (type == TFO_POINT)
	{
		return  Vulkan_FILTER_MIN_MAG_MIP_POINT;
	}
	else if (type == TFO_BILINEAR)
	{
		return Vulkan_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	}
	else if (type == TFO_TRILINEAR)
	{
		return Vulkan_FILTER_MIN_MAG_MIP_LINEAR;
	}
	else if (type == TFO_ANISOTROPIC)
	{
		return Vulkan_FILTER_ANISOTROPIC;
	}
	else if (type == TFO_SHADOWCOMPARE)
	{
		return Vulkan_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	}
	else
	{
		return Vulkan_FILTER_MIN_MAG_MIP_POINT;
		ASSERT(false);
	}
}


Vulkan_TEXTURE_ADDRESS_MODE		VulkanMapping::GetVulkanWrap(Wrap type)
{
	if (type == REPEAT)
	{
		return Vulkan_TEXTURE_ADDRESS_WRAP;
	}
	else if (type == CLAMP)
	{
		return Vulkan_TEXTURE_ADDRESS_CLAMP;
	}
	else 
	{
		ASSERT(false);
	}

	return Vulkan_TEXTURE_ADDRESS_WRAP;
}

Vulkan_COMPARISON_FUNC VulkanMapping::get(CompareFunction cf)
{
	switch( cf )
	{
	case CMPF_ALWAYS_FAIL:
		return Vulkan_COMPARISON_NEVER;
	case CMPF_ALWAYS_PASS:
		return Vulkan_COMPARISON_ALWAYS;
	case CMPF_LESS:
		return Vulkan_COMPARISON_LESS;
	case CMPF_LESS_EQUAL:
		return Vulkan_COMPARISON_LESS_EQUAL;
	case CMPF_EQUAL:
		return Vulkan_COMPARISON_EQUAL;
	case CMPF_NOT_EQUAL:
		return Vulkan_COMPARISON_NOT_EQUAL;
	case CMPF_GREATER_EQUAL:
		return Vulkan_COMPARISON_GREATER_EQUAL;
	case CMPF_GREATER:
		return Vulkan_COMPARISON_GREATER;
	default:
		ASSERT(false);
		return Vulkan_COMPARISON_ALWAYS;
	};
}

Vulkan_STENCIL_OP VulkanMapping::get(StencilOperation op, bool invert)
{
	switch(op)
	{
	case SOP_KEEP:
		return Vulkan_STENCIL_OP_KEEP;
	case SOP_ZERO:
		return Vulkan_STENCIL_OP_ZERO;
	case SOP_REPLACE:
		return Vulkan_STENCIL_OP_REPLACE;
	case SOP_INCREMENT:
		return invert? Vulkan_STENCIL_OP_DECR_SAT : Vulkan_STENCIL_OP_INCR_SAT;
	case SOP_DECREMENT:
		return invert? Vulkan_STENCIL_OP_INCR_SAT : Vulkan_STENCIL_OP_DECR_SAT;
	case SOP_INCREMENT_WRAP:
		return invert? Vulkan_STENCIL_OP_DECR : Vulkan_STENCIL_OP_INCR;
	case SOP_DECREMENT_WRAP:
		return invert? Vulkan_STENCIL_OP_INCR : Vulkan_STENCIL_OP_DECR;
	case SOP_INVERT:
		return Vulkan_STENCIL_OP_INVERT;
	default:
		ASSERT(false);
		return Vulkan_STENCIL_OP_KEEP;
	}
}

void VulkanMapping::GetD3DBlend(BLEND_MODE mode,BOOL& bEnbale,Vulkan_BLEND& src,Vulkan_BLEND& dest,Vulkan_BLEND_OP& op)
{
	switch (mode)
	{
	case BM_OPATICY:
		bEnbale = FALSE;
		break;

	case BM_TRANSPARENT:
		bEnbale = TRUE;
		src = Vulkan_BLEND_SRC_ALPHA;
		dest = Vulkan_BLEND_INV_SRC_ALPHA;
		op = Vulkan_BLEND_OP_ADD;
		break;

	case BM_ADD:
		bEnbale = TRUE;
		src = Vulkan_BLEND_ONE;
		dest = Vulkan_BLEND_ONE;
		op = Vulkan_BLEND_OP_ADD;
		break;

	case BM_MULTIPLY:
		bEnbale = TRUE;
		src = Vulkan_BLEND_ZERO;
		dest = Vulkan_BLEND_SRC_COLOR;
		op = Vulkan_BLEND_OP_ADD;
		break;
	default:
		ASSERT(false);
		bEnbale = FALSE;
	}
}

void VulkanMapping::GetD3DBlendAlpha(BLEND_MODE mode,BOOL& bEnbale,Vulkan_BLEND& src,Vulkan_BLEND& dest,Vulkan_BLEND_OP& op)
{
	switch (mode)
	{
	case BM_OPATICY:
		bEnbale = FALSE;
		break;

	case BM_TRANSPARENT:
		bEnbale = TRUE;
		src = Vulkan_BLEND_SRC_ALPHA;
		dest = Vulkan_BLEND_INV_SRC_ALPHA;
		op = Vulkan_BLEND_OP_ADD;
		break;

	case BM_ADD:
		bEnbale = TRUE;
		src = Vulkan_BLEND_ONE;
		dest = Vulkan_BLEND_ONE;
		op = Vulkan_BLEND_OP_ADD;
		break;

	case BM_MULTIPLY:
		bEnbale = TRUE;
		src = Vulkan_BLEND_ZERO;
		dest = Vulkan_BLEND_SRC_ALPHA;
		op = Vulkan_BLEND_OP_ADD;
		break;
	default:
		ASSERT(false);
		bEnbale = FALSE;
	}
}

Vulkan_FILL_MODE VulkanMapping::get(FillMode mode)
{
	if (mode == FILL_SOLID)
	{
		return Vulkan_FILL_SOLID;
	}
	else if (mode == FILL_WIREFRAME)
	{
		return Vulkan_FILL_WIREFRAME;
	}
	else if (mode == FILL_POINT)
	{
		return Vulkan_FILL_WIREFRAME;
	}
	else
	{
		ASSERT(FALSE);
		return Vulkan_FILL_SOLID;
	}
}


Vulkan_CULL_MODE VulkanMapping::get(CULL_MODE mode)
{
	if (mode == CULL_FACE_SIDE_NONE)
	{
		return Vulkan_CULL_NONE;
	}
	else if (mode == CULL_FACE_SIDE_BACK)
	{
		return Vulkan_CULL_BACK;
	}
	else if (mode == CULL_FACE_SIDE_FRONT)
	{
		return Vulkan_CULL_FRONT;
	}
	else
	{
		ASSERT(FALSE);
		return Vulkan_CULL_BACK;
	}
}

}
