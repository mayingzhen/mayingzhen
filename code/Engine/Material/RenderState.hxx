#include "RenderState.h"


namespace ma
{

	RenderState::RenderState()
	{
		m_bDepthWrite = true;
		m_bColorWrite = true;
		m_fConstantBias = 0;
		m_fSlopeScaleBias = 0;
		m_eDepthCheckMode = CMPF_LESS_EQUAL;
		m_eBlendMode = BM_OPATICY;
		m_eCullMode = CULL_FACE_SIDE_BACK;

		m_bStencil = false;
		m_eStencilfunc = CMPF_ALWAYS_PASS;
		m_eStencilPass = SOP_KEEP;
		m_eStencilFail = SOP_KEEP;
		m_eDepthFailOp = SOP_KEEP;
		m_nStencilRefValue = -1;
		m_nStencilMask = -1;
		m_nStencilWriteMask = -1;

		m_eFillMode = FILL_SOLID;
		m_bScissorTest = false;
	}


	void RenderState::RegisterAttribute()
	{
		ATTRIBUTE(RenderState, "DepthWrite", m_bDepthWrite, bool, true, AM_DEFAULT);
		ATTRIBUTE(RenderState, "ColorWrite", m_bColorWrite, bool, true, AM_DEFAULT);
		ENUM_ATTRIBUTE(RenderState, "DepthCheckMode", m_eDepthCheckMode, CompareFunction, strCompareFunction,CMPF_LESS_EQUAL, AM_DEFAULT);
		ENUM_ATTRIBUTE(RenderState, "BlendMode", m_eBlendMode, BLEND_MODE, strBlendMode,BM_OPATICY, AM_DEFAULT);
		ENUM_ATTRIBUTE(RenderState, "CullMode", m_eCullMode, CULL_MODE, strCullMode,CULL_FACE_SIDE_BACK, AM_DEFAULT);

		ATTRIBUTE(RenderState, "Stencil", m_bStencil, bool, false, AM_DEFAULT);
		ENUM_ATTRIBUTE(RenderState, "Stencilfunc", m_eStencilfunc, CompareFunction, strCompareFunction,CMPF_ALWAYS_PASS, AM_DEFAULT);
		ENUM_ATTRIBUTE(RenderState, "StencilPass", m_eStencilPass, StencilOperation, strStencilOperation,SOP_KEEP, AM_DEFAULT);
		ENUM_ATTRIBUTE(RenderState, "StencilFail ", m_eStencilFail , StencilOperation, strStencilOperation,SOP_KEEP, AM_DEFAULT);
		ENUM_ATTRIBUTE(RenderState, "DepthFailOp", m_eDepthFailOp, StencilOperation, strStencilOperation,SOP_KEEP, AM_DEFAULT);
		ATTRIBUTE(RenderState, "StencilRefValue", m_nStencilRefValue, uint32, -1, AM_DEFAULT);
		ATTRIBUTE(RenderState, "StencilMask", m_nStencilMask, uint32, -1, AM_DEFAULT);
		ATTRIBUTE(RenderState, "WriteMask", m_nStencilWriteMask, uint32, -1, AM_DEFAULT);
		
	}


	bool RenderState::GetTransparent() const
	{
		return m_eBlendMode != BM_OPATICY;
	}


}