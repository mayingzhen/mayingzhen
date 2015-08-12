#ifndef _RenderState_H_
#define _RenderState_H_

#include "Engine/RenderSystem/IRenderDevice/RenderDefine.h"


namespace ma
{
	class  RenderState : public Object
	{

	public:

		RenderState();

		DECL_OBJECT(RenderState);

		static void			RegisterAttribute();

		bool GetDepthWrite() const {return m_bDepthWrite;}
		void SetDepthWrite(bool bDepthWrite) {m_bDepthWrite = bDepthWrite;}

		bool GetColorWrite() const {return m_bColorWrite;}
		void SetColorWrite(bool bColorWrite) {m_bColorWrite = bColorWrite;}
		
		float GetDepthBias() const {return m_fDepthBias;}
		void SetDepthBias(float fDepthBias) {m_fDepthBias = fDepthBias;}

		DEPTH_CHECK_MODE GetDepthCheckMode() const {return m_eDepthCheckMode;}
		void SetDepthCheckMode(DEPTH_CHECK_MODE eDepthCheckMode) {m_eDepthCheckMode = eDepthCheckMode;}

		BLEND_MODE GetBlendMode() const {return m_eBlendMode;}
		void SetBlendMode(BLEND_MODE eBlendMode) {m_eBlendMode = eBlendMode;}

		CULL_MODE GetCullMode() const {return m_eCullMode;}
		void SetCullMode(CULL_MODE eCullMode) {m_eCullMode = eCullMode;}

	private:
		bool				m_bDepthWrite;
		bool				m_bColorWrite;
		float				m_fDepthBias;
		DEPTH_CHECK_MODE	m_eDepthCheckMode;
		BLEND_MODE			m_eBlendMode;
		CULL_MODE			m_eCullMode;

	};
		
}




#endif