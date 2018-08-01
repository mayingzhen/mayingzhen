#pragma once


namespace ma
{

	class VulkanMapping
	{
	public:
		static VkIndexType	GetIndexType(INDEX_TYPE eType);
		static VkFormat  _getPF(PixelFormat Format);
		static VkFilter  GetFilter(Filter type);
		static VkSamplerAddressMode	GetWrap(Wrap type);
		static const char*  GetDeclUsage(DECL_USAGE DeclUsage);
		static VkFormat  GetDeclType(DECL_TYPE DeclType);
		static VkPrimitiveTopology GetPrimitiveType(PRIMITIVE_TYPE type);

		static VkCompareOp get(CompareFunction cf);

		static VkStencilOp get(StencilOperation op, bool invert = false);

		static VkPolygonMode get(FillMode mode);

		static VkCullModeFlagBits get(CULL_MODE mode);

		static VkBlendOp get(BLEND_OP op);

		static VkBlendFactor get(BLEND blend);

		static void GetBlend(BLEND_MODE mode, bool& bEnbale, VkBlendFactor& src, VkBlendFactor& dest, VkBlendOp& op);
		static void GetBlendAlpha(BLEND_MODE mode, bool& bEnbale, VkBlendFactor& src, VkBlendFactor& dest, VkBlendOp& op);

		static PixelFormat _getClosestSupportedPF(PixelFormat ogrePF);
	};

} 


