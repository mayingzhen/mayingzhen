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


	void RenderState::Serialize(Serializer& sl, const char* pszLable/* = "RenderState"*/)
	{
		sl.BeginSection(pszLable);
	
		sl.Serialize(m_bDepthWrite,"DepthWrite");
		sl.Serialize(m_bColorWrite,"ColorWrite");
		sl.Serialize(m_fDepthBias,"DepthBias");
		sl.Serialize(m_eDepthCheckMode,strDepthCheckMode,"DepthCheckMode");
		sl.Serialize(m_eBlendMode,strBlendMode,"BlendMode");
		sl.Serialize(m_eCullMode,strCullMode,"CullMode");

		sl.EndSection();
	}

}