#include "RenderScheme.h"

namespace ma
{
	static RenderScheme* gpRenderScheme = NULL;

	RenderScheme* GetRenderScheme()
	{
		return gpRenderScheme;
	}

	void SetRenderScheme(RenderScheme* pRenderScheme)
	{
		gpRenderScheme = pRenderScheme;
	}


	RenderScheme::RenderScheme()
	{
		std::list<RenderPass*>::iterator it = m_arrRenderPass.begin();
		for (; it != m_arrRenderPass.end(); ++it)
		{
			RenderPass* pRenderPass = *it;
			if (pRenderPass == NULL)
				continue;

			pRenderPass->Init();
		}	
	}

	void RenderScheme::Init()
	{
		std::list<RenderPass*>::iterator it = m_arrRenderPass.begin();
		for (; it != m_arrRenderPass.end(); ++it)
		{
			RenderPass* pRenderPass = *it;
			if (pRenderPass == NULL)
				continue;

			pRenderPass->Init();
		}	
	}

	void RenderScheme::ShoutDown()
	{
		std::list<RenderPass*>::iterator it = m_arrRenderPass.begin();
		for (; it != m_arrRenderPass.end(); ++it)
		{
			RenderPass* pRenderPass = *it;
			if (pRenderPass == NULL)
				continue;

			pRenderPass->ShoutDown();
		}	
	}

	void RenderScheme::Render()
	{
		std::list<RenderPass*>::iterator it = m_arrRenderPass.begin();
		for (; it != m_arrRenderPass.end(); ++it)
		{
			RenderPass* pRenderPass = *it;
			if (pRenderPass == NULL)
				continue;

			pRenderPass->Render();
		}	
	}

	void RenderScheme::AddRenderPass(RenderPass* pPass,bool bFront/* = false*/)
	{
		if (!bFront)
		{
			m_arrRenderPass.push_back(pPass);
		}
		else
		{
			m_arrRenderPass.push_front(pPass);
		}
	}


// 	RenderScheme::RenderScheme()
// 	{	
// 		m_pShadowDepthPas = new ShadowDepthPass();
// 		GetRenderSystem()->AddRenderPass(m_pShadowDepthPas);
// 
// 		m_pGBufferPass = new GBufferPass();
// 		GetRenderSystem()->AddRenderPass(m_pGBufferPass);
// 
// 		m_pShadowPass = new DeferredShadowPass();
// 		GetRenderSystem()->AddRenderPass(m_pShadowPass);
// 
// 		m_pLightPass = new DeferredLightPass();
// 		GetRenderSystem()->AddRenderPass(m_pLightPass);
// 
// 		m_pShadingPass = new ShadingPass();
// 		GetRenderSystem()->AddRenderPass(m_pShadingPass);
// 	}

}
