#include "D3D11RenderState.h"

namespace ma
{
	D3D11RasterizerStateObject::D3D11RasterizerStateObject(const RasterizerStateDesc& desc)
		:RasterizerStateObject(desc)
	{
// 		D3D11RenderDevice& re = D3D11RenderDevice*(GetRenderDevice());
// 		ID3D11Device* d3d_device = re.D3DDevice().get();
// 
// 		D3D11_RASTERIZER_DESC d3d_desc;
// 		d3d_desc.FillMode = D3D11Mapping::get(desc.m_eFillMode);
// 		d3d_desc.CullMode = D3D11Mapping::get(desc.m_eCullMode);
// 		d3d_desc.FrontCounterClockwise = desc.front_face_ccw;
// 		d3d_desc.DepthBias = static_cast<int>(desc.polygon_offset_units);
// 		d3d_desc.DepthBiasClamp = desc.polygon_offset_units;
// 		d3d_desc.SlopeScaledDepthBias = desc.polygon_offset_factor;
// 		d3d_desc.DepthClipEnable = re.DeviceFeatureLevel() >= D3D_FEATURE_LEVEL_10_0 ? desc.depth_clip_enable : true;
// 		d3d_desc.ScissorEnable = desc.scissor_enable;
// 		d3d_desc.MultisampleEnable = desc.multisample_enable;
// 		d3d_desc.AntialiasedLineEnable = false;
// 
// 		if (re.D3D11RuntimeSubVer() >= 1)
// 		{
// 			ID3D11Device1* d3d_device_1 = static_cast<ID3D11Device1*>(d3d_device);
// 			D3D11_RASTERIZER_DESC1 d3d_desc1;
// 			d3d_desc1.FillMode = d3d_desc.FillMode;
// 			d3d_desc1.CullMode = d3d_desc.CullMode;
// 			d3d_desc1.FrontCounterClockwise = d3d_desc.FrontCounterClockwise;
// 			d3d_desc1.DepthBias = d3d_desc.DepthBias;
// 			d3d_desc1.DepthBiasClamp = d3d_desc.DepthBiasClamp;
// 			d3d_desc1.SlopeScaledDepthBias = d3d_desc.SlopeScaledDepthBias;
// 			d3d_desc1.DepthClipEnable = d3d_desc.DepthClipEnable;
// 			d3d_desc1.ScissorEnable = d3d_desc.ScissorEnable;
// 			d3d_desc1.MultisampleEnable = d3d_desc.MultisampleEnable;
// 			d3d_desc1.AntialiasedLineEnable = d3d_desc.AntialiasedLineEnable;
// 			d3d_desc1.ForcedSampleCount = 0;
// 
// 			ID3D11RasterizerState1* rasterizer_state;
// 			TIF(d3d_device_1->CreateRasterizerState1(&d3d_desc1, &rasterizer_state));
// 			rasterizer_state_ = MakeCOMPtr(rasterizer_state);
// 		}
// 		else
// 		{
// 			ID3D11RasterizerState* rasterizer_state;
// 			TIF(d3d_device->CreateRasterizerState(&d3d_desc, &rasterizer_state));
// 			rasterizer_state_ = MakeCOMPtr(rasterizer_state);
// 		}
	}

	void D3D11RasterizerStateObject::Active()
	{

	}

	D3D11DepthStencilStateObject::D3D11DepthStencilStateObject(const DepthStencilStateDesc& desc)
		:DepthStencilStateObject(desc)
	{

	}

	void D3D11DepthStencilStateObject::Active(UINT32 front_stencil_ref, UINT32 back_stencil_ref)
	{

	}

	D3D11BlendStateObject::D3D11BlendStateObject(const BlendStateDesc& desc)
		:BlendStateObject(desc)
	{

	}

	void D3D11BlendStateObject::Active(const ColourValue& blend_factor, UINT32 sample_mask)
	{

	}
}
