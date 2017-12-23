#include "VulkanMapping.h"

namespace ma
{

VkIndexType VulkanMapping::GetIndexType(INDEX_TYPE eType)
{
	if (eType == INDEX_TYPE_U16)
	{
		return VK_INDEX_TYPE_UINT16;
	}
	else
	{
		return VK_INDEX_TYPE_UINT32;
	}
}

PixelFormat VulkanMapping::_getClosestSupportedPF(PixelFormat ogrePF)
{
	if (_getPF(ogrePF) != VK_FORMAT_UNDEFINED)
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
		return PF_A8R8G8B8;
	}
}



VkFormat VulkanMapping::_getPF(PixelFormat ogrePF)
{
	switch(ogrePF)
	{
	case PF_A8R8G8B8: 
		return VK_FORMAT_B8G8R8A8_UNORM;
	case PF_A2B10G10R10:
		return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
	case PF_A2R10G10B10:
		return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
	case PF_FLOAT16_R:
		return VK_FORMAT_R16_SFLOAT;
	case PF_FLOAT16_RGBA:
		return VK_FORMAT_R16G16B16A16_SFLOAT;
	case PF_FLOAT32_R:
		return VK_FORMAT_R32_SFLOAT;
	case PF_FLOAT32_RGBA:
		return VK_FORMAT_R32G32B32A32_SFLOAT;
	case PF_DXT1:
		return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
	case PF_DXT2:
		return VK_FORMAT_BC2_UNORM_BLOCK;
	case PF_DXT3:
		return VK_FORMAT_BC3_UNORM_BLOCK;
	case PF_DXT4:
		return VK_FORMAT_BC3_UNORM_BLOCK;
	case PF_DXT5:
		return VK_FORMAT_BC3_UNORM_BLOCK;
	case PF_D24S8:
		return VK_FORMAT_D24_UNORM_S8_UINT;
	case PF_D16F:
		return VK_FORMAT_D16_UNORM;
	case PF_UNKNOWN:
	default:
		return VK_FORMAT_UNDEFINED;
	}
}


VkFormat VulkanMapping::GetDeclType(DECL_TYPE DeclType)
{
    switch (DeclType)
    {
    case DT_FLOAT1:
        return VK_FORMAT_R32_SFLOAT;

    case DT_FLOAT2:
        return VK_FORMAT_R32G32_SFLOAT;

    case DT_FLOAT3:
        return VK_FORMAT_R32G32B32_SFLOAT;

    case DT_FLOAT4:
        return VK_FORMAT_R32G32B32A32_SFLOAT;

    case DT_COLOR:
        return VK_FORMAT_R8G8B8A8_UNORM;

    case DT_UBYTE4:
		return VK_FORMAT_R8G8B8A8_UINT;

	case  DT_UBYTE4N:
		return VK_FORMAT_R8G8B8A8_UNORM;

    case DT_SHORT2:
		return VK_FORMAT_R16G16_SINT;

	case DT_SHORT2N:
		return VK_FORMAT_R16G16_SNORM;

    case DT_SHORT4:
		return VK_FORMAT_R16G16B16A16_SINT;

	case DT_SHORT4N:
		return VK_FORMAT_R16G16B16A16_SNORM;

    default:
        ASSERT(0 && "Invalid Declaration Type.");
        return VK_FORMAT_UNDEFINED;
    }
}


VkPrimitiveTopology VulkanMapping::GetPrimitiveType(PRIMITIVE_TYPE type)
{
	switch (type)
	{
	case PRIM_POINTLIST:
		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			  
	case PRIM_LINELIST:
		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

	case PRIM_LINESTRIP:
		return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

	case PRIM_TRIANGLELIST:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	case PRIM_TRIANGLESTRIP:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

	case PRIM_TRIANGLEFAN:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

	default:
		ASSERT(0 && "Invalid Declaration Usage.");
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	} 
}


const char* VulkanMapping::GetDeclUsage(DECL_USAGE DeclUsage)
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


VkFilter VulkanMapping::GetFilter(Filter type)
{
	if (type == TFO_POINT)
	{
		return  VK_FILTER_NEAREST;
	}
	else if (type == TFO_BILINEAR)
	{
		return VK_FILTER_LINEAR;
	}
	else if (type == TFO_TRILINEAR)
	{
		return VK_FILTER_CUBIC_IMG;
	}
	else if (type == TFO_ANISOTROPIC)
	{
		return VK_FILTER_CUBIC_IMG;
	}
	else if (type == TFO_SHADOWCOMPARE)
	{
		return VK_FILTER_CUBIC_IMG;
	}
	else
	{
		return VK_FILTER_CUBIC_IMG;
		ASSERT(false);
	}
}


VkSamplerAddressMode VulkanMapping::GetWrap(Wrap type)
{
	if (type == REPEAT)
	{
		return VK_SAMPLER_ADDRESS_MODE_REPEAT;
	}
	else if (type == CLAMP)
	{
		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	}
	else 
	{
		ASSERT(false);
	}

	return VK_SAMPLER_ADDRESS_MODE_REPEAT;
}

VkCompareOp VulkanMapping::get(CompareFunction cf)
{
	switch( cf )
	{
	case CMPF_ALWAYS_FAIL:
		return VK_COMPARE_OP_NEVER;
	case CMPF_ALWAYS_PASS:
		return VK_COMPARE_OP_ALWAYS;
	case CMPF_LESS:
		return VK_COMPARE_OP_LESS;
	case CMPF_LESS_EQUAL:
		return VK_COMPARE_OP_LESS_OR_EQUAL;
	case CMPF_EQUAL:
		return VK_COMPARE_OP_EQUAL;
	case CMPF_NOT_EQUAL:
		return VK_COMPARE_OP_NOT_EQUAL;
	case CMPF_GREATER_EQUAL:
		return VK_COMPARE_OP_EQUAL;
	case CMPF_GREATER:
		return VK_COMPARE_OP_GREATER;
	default:
		ASSERT(false);
		return VK_COMPARE_OP_LESS;
	};
}

VkStencilOp VulkanMapping::get(StencilOperation op, bool invert)
{
	switch(op)
	{
	case SOP_KEEP:
		return VK_STENCIL_OP_KEEP;
	case SOP_ZERO:
		return VK_STENCIL_OP_ZERO;
	case SOP_REPLACE:
		return VK_STENCIL_OP_REPLACE;
	case SOP_INCREMENT:
		return VK_STENCIL_OP_INVERT;
	case SOP_DECREMENT:
		return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
	case SOP_INCREMENT_WRAP:
		return VK_STENCIL_OP_INCREMENT_AND_WRAP;
	case SOP_DECREMENT_WRAP:
		return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
	case SOP_INVERT:
		return VK_STENCIL_OP_INVERT;
	default:
		ASSERT(false);
		return VK_STENCIL_OP_KEEP;
	}
}

void VulkanMapping::GetBlend(BLEND_MODE mode, bool& bEnbale, VkBlendFactor& src, VkBlendFactor& dest, VkBlendOp& op)
{
	switch (mode)
	{
	case BM_OPATICY:
		bEnbale = false;
		break;

	case BM_TRANSPARENT:
		bEnbale = true;
		src = VK_BLEND_FACTOR_SRC_ALPHA;
		dest = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		op = VK_BLEND_OP_ADD;
		break;

	case BM_ADD:
		bEnbale = true;
		src = VK_BLEND_FACTOR_ONE;
		dest = VK_BLEND_FACTOR_ONE;
		op = VK_BLEND_OP_ADD;
		break;

	case BM_MULTIPLY:
		bEnbale = true;
		src = VK_BLEND_FACTOR_ZERO;
		dest = VK_BLEND_FACTOR_SRC_COLOR;
		op = VK_BLEND_OP_ADD;
		break;
	default:
		ASSERT(false);
		bEnbale = false;
	}
}

void VulkanMapping::GetBlendAlpha(BLEND_MODE mode, bool& bEnbale, VkBlendFactor& src, VkBlendFactor& dest, VkBlendOp& op)
{
	switch (mode)
	{
	case BM_OPATICY:
		bEnbale = false;
		break;

	case BM_TRANSPARENT:
		bEnbale = true;
		src = VK_BLEND_FACTOR_SRC_ALPHA;
		dest = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		op = VK_BLEND_OP_ADD;
		break;

	case BM_ADD:
		bEnbale = true;
		src = VK_BLEND_FACTOR_ONE;
		dest = VK_BLEND_FACTOR_ONE;
		op = VK_BLEND_OP_ADD;
		break;

	case BM_MULTIPLY:
		bEnbale = true;
		src = VK_BLEND_FACTOR_ZERO;
		dest = VK_BLEND_FACTOR_SRC_ALPHA;
		op = VK_BLEND_OP_ADD;
		break;
	default:
		ASSERT(false);
		bEnbale = false;
	}
}

VkPolygonMode VulkanMapping::get(FillMode mode)
{
	if (mode == FILL_SOLID)
	{
		return VK_POLYGON_MODE_FILL;
	}
	else if (mode == FILL_WIREFRAME)
	{
		return VK_POLYGON_MODE_LINE;
	}
	else if (mode == FILL_POINT)
	{
		return VK_POLYGON_MODE_POINT;
	}
	else
	{
		ASSERT(false);
		return VK_POLYGON_MODE_FILL;
	}
}


VkCullModeFlagBits VulkanMapping::get(CULL_MODE mode)
{
	if (mode == CULL_FACE_SIDE_NONE)
	{
		return VK_CULL_MODE_NONE;
	}
	else if (mode == CULL_FACE_SIDE_BACK)
	{
		return VK_CULL_MODE_BACK_BIT;
	}
	else if (mode == CULL_FACE_SIDE_FRONT)
	{
		return VK_CULL_MODE_FRONT_BIT;
	}
	else
	{
		ASSERT(false);
		return VK_CULL_MODE_BACK_BIT;
	}
}

}
