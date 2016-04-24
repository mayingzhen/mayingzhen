#ifndef _RenderState_H_
#define _RenderState_H_

#include "Engine/RenderSystem/IRenderDevice/RenderDefine.h"


namespace ma
{
	class RenderState : public Serializable
	{
	public:
		RenderState();

		DECL_OBJECT(RenderState);

		static void	RegisterAttribute();

		bool GetTransparent(void) const;

	public:
		bool				m_bColorWrite;

		BLEND_MODE			m_eBlendMode;
		CULL_MODE			m_eCullMode;	
		bool				m_bSRGBWrite;

		// Depth
		bool				m_bDepthWrite;
		CompareFunction		m_eDepthCheckMode;

		float				m_fConstantBias;
		float				m_fSlopeScaleBias;

		// Stencil
		bool				m_bStencil;
		CompareFunction		m_eStencilfunc;
		StencilOperation	m_eStencilPass;
		StencilOperation	m_eStencilFail;
		StencilOperation	m_eDepthFailOp;
		uint32				m_nStencilRefValue;
		uint32              m_nStencilMask;
		uint32				m_nStencilWriteMask;

		bool				m_bScissorTest;
		FillMode			m_eFillMode;
	};
		
}




#endif