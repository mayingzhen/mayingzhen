#include "RenderState.h"


namespace ma
{

	RenderState::RenderState()
	{
		m_bDepthWrite = true;
		m_bColorWrite = true;
		m_fConstantBias = 0;
		m_fSlopeScaleBias = 0;
		m_eDepthCheckMode = DCM_LESS_EQUAL;
		m_eBlendMode = BM_OPATICY;
		m_eCullMode = CULL_FACE_SIDE_BACK;
	}


	void RenderState::RegisterAttribute()
	{
		ACCESSOR_ATTRIBUTE(RenderState, "DepthWrite", GetDepthWrite, SetDepthWrite, bool, true, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(RenderState, "ColorWrite", GetColorWrite, SetColorWrite, bool, true, AM_DEFAULT);
		ENUM_ACCESSOR_ATTRIBUTE(RenderState, "DepthCheckMode", GetDepthCheckMode, SetDepthCheckMode, DEPTH_CHECK_MODE, strDepthCheckMode,DCM_LESS_EQUAL, AM_DEFAULT);
		ENUM_ACCESSOR_ATTRIBUTE(RenderState, "BlendMode", GetBlendMode, SetBlendMode, BLEND_MODE, strBlendMode,BM_OPATICY, AM_DEFAULT);
		ENUM_ACCESSOR_ATTRIBUTE(RenderState, "CullMode", GetCullMode, SetCullMode, CULL_MODE, strCullMode,CULL_FACE_SIDE_BACK, AM_DEFAULT);
	}


	bool RenderState::GetTransparent() const
	{
		return m_eBlendMode != BM_OPATICY;
	}


}