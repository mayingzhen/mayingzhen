#include "DefferredRender.h"

namespace ma
{

	DefferredRender::DefferredRender()
	{	
		if (GetRenderSetting()->m_bShadow)
		{
			m_pShadowDepthPas = new ShadowDepthPass();
			AddRenderPass(m_pShadowDepthPas);
		}

		if (GetRenderSetting()->m_bDefferLight)
		{
			m_pGBufferPass = new GBufferPass();
			AddRenderPass(m_pGBufferPass);
		}

		if ( GetRenderSetting()->m_bShadow )
		{
			m_pShadowPass = new DeferredShadowPass();
			AddRenderPass(m_pShadowPass);
		}

		if (GetRenderSetting()->m_bDefferLight)
		{
			m_pLightPass = new DeferredLightPass();
			AddRenderPass(m_pLightPass);
		}

		m_pShadingPass = new ShadingPass();
		AddRenderPass(m_pShadingPass);
	}

}
