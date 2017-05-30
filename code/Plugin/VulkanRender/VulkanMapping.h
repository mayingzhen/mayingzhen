#pragma once


namespace ma
{

	class VulkanMapping
	{
	public:
		static DXGI_FORMAT	GetD3DIndexType(INDEX_TYPE eType);
		static DXGI_FORMAT  _getPF(PixelFormat Format);
		static Vulkan_FILTER  GetVulkanFilter(Filter type);
		static Vulkan_TEXTURE_ADDRESS_MODE		GetVulkanWrap(Wrap type);
		static const char*  GetD3DDeclUsage(DECL_USAGE DeclUsage);
		static DXGI_FORMAT  GetD3DDeclType(DECL_TYPE DeclType);
		static D3D_PRIMITIVE_TOPOLOGY GetD3DPrimitiveType(PRIMITIVE_TYPE type);

		static Vulkan_COMPARISON_FUNC get(CompareFunction cf);

		static Vulkan_STENCIL_OP get(StencilOperation op, bool invert = false);

		static Vulkan_FILL_MODE get(FillMode mode);

		static Vulkan_CULL_MODE get(CULL_MODE mode);

		static void GetD3DBlend(BLEND_MODE mode,BOOL& bEnbale,Vulkan_BLEND& src,Vulkan_BLEND& destm,Vulkan_BLEND_OP& op);
		static void GetD3DBlendAlpha(BLEND_MODE mode,BOOL& bEnbale,Vulkan_BLEND& src,Vulkan_BLEND& destm,Vulkan_BLEND_OP& op);

		static PixelFormat _getClosestSupportedPF(PixelFormat ogrePF);
	};

} 


