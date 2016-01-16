#include "RenderState.h"


namespace ma
{

	static const char* strDepthCheckMode[] =
	{
		"DCM_LESS_EQUAL",
		"DCM_LESS",
		"DCM_GREATER_EQUAL",
		"DCM_GREATER",
		"DCM_EQUAL",
		"DCM_ALWAYS",
		"DCM_NONE",
		0
	};

	static const char* strBlendMode[] =
	{
		"BM_OPATICY",
		"BM_TRANSPARENT",
		"BM_ADD",
		"BM_MULTIPLY",
		0
	};

	static const char* strCullMode[] =
	{
		"CULL_FACE_SIDE_NONE",
		"CULL_FACE_SIDE_BACK",
		"CULL_FACE_SIDE_FRONT",
		"CULL_FACE_SIDE_FRONT_AND_BACK",
		0
	};

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



}