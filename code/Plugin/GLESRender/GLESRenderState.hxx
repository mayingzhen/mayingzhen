#include "GLESRenderState.h"

namespace ma
{

	void GLESRasterizerStateObject::RT_StreamComplete()
	{
		//GetD3D11DxDevive()->CreateO
	}

	void GLESDepthStencilStateObject::RT_StreamComplete(/*UINT32 front_stencil_ref, UINT32 back_stencil_ref*/)
	{
		//m_pD3D11DSState = GetD3D11DxDeviveContext()->CreateOrGetDepthStencilState(this);
// 		unsigned newDepthStateHash =
// 			(pDSState->m_bDepthWrite ? 1 : 0) |
// 			(pDSState->m_bStencil ? 2 : 0) |
// 			(pDSState->m_eDepthCheckMode << 2) |
// 			((pDSState->m_nStencilMask & 0xff) << 5) |
// 			((pDSState->m_nStencilWriteMask & 0xff) << 13) | (pDSState->m_eStencilfunc << 21) |
// 			((pDSState->m_eStencilFail + pDSState->m_eDepthFailOp * 5 + pDSState->m_eStencilPass * 25) << 24);
// 
// 		map<unsigned, ID3D11DepthStencilState*>::iterator i = m_depthStatePool.find(newDepthStateHash);
// 		if (i == m_depthStatePool.end())
// 		{
// 			D3D11_DEPTH_STENCIL_DESC stateDesc;
// 			memset(&stateDesc, 0, sizeof stateDesc);
// 			stateDesc.DepthEnable = TRUE;
// 			stateDesc.DepthWriteMask = pDSState->m_bDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
// 			stateDesc.DepthFunc = D3D11Mapping::get(pDSState->m_eDepthCheckMode); ;
// 			stateDesc.StencilEnable = pDSState->m_bStencil ? TRUE : FALSE;
// 			stateDesc.StencilReadMask = (unsigned char)pDSState->m_nStencilMask;
// 			stateDesc.StencilWriteMask = (unsigned char)pDSState->m_nStencilWriteMask;
// 			stateDesc.FrontFace.StencilFailOp = D3D11Mapping::get(pDSState->m_eStencilFail);
// 			stateDesc.FrontFace.StencilDepthFailOp = D3D11Mapping::get(pDSState->m_eDepthFailOp);
// 			stateDesc.FrontFace.StencilPassOp = D3D11Mapping::get(pDSState->m_eStencilPass);
// 			stateDesc.FrontFace.StencilFunc = D3D11Mapping::get(pDSState->m_eStencilfunc);
// 
// 			// BackFace
// 			stateDesc.BackFace.StencilFailOp = D3D11Mapping::get(pDSState->m_eStencilFail);
// 			stateDesc.BackFace.StencilDepthFailOp = D3D11Mapping::get(pDSState->m_eStencilFail);
// 			stateDesc.BackFace.StencilPassOp = D3D11Mapping::get(pDSState->m_eStencilFail);
// 			stateDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
// 
// 			ID3D11DepthStencilState* newDepthState = 0;
// 			m_pD3DDevice->CreateDepthStencilState(&stateDesc, &newDepthState);
// 			if (!newDepthState)
// 				LogError("Failed to create depth state");
// 
// 			m_depthStatePool.insert(std::make_pair(newDepthStateHash, newDepthState));
// 
// 			return newDepthState;
// 		}
// 		else
// 		{
// 			return it->second;
// 		}
	}

	void GLESBlendStateObject::RT_StreamComplete(/*const ColourValue& blend_factor, UINT32 sample_mask*/)
	{

	}
}
