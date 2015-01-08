#ifndef _RenderState_H_
#define _RenderState_H_

#include "Engine/RenderSystem/IRenderDevice/RenderDefine.h"

namespace ma
{
	struct RENDER_API RenderState 
	{
		bool				m_bDepthWrite;
		bool				m_bColorWrite;
		float				m_fDepthBias;
		DEPTH_CHECK_MODE	m_eDepthCheckMode;
		BLEND_MODE			m_eBlendMode;
		CULL_MODE			m_eCullMode;

		RenderState()
		{
			m_bDepthWrite = true;
			m_bColorWrite = true;
			m_fDepthBias = 0;
			m_eDepthCheckMode = DCM_LESS_EQUAL;
			m_eBlendMode = BM_OPATICY;
			m_eCullMode = CULL_FACE_SIDE_BACK;
		}

		void	Serialize(Serializer& sl, const char* pszLable = "RenderState");
	};
		
}




#endif