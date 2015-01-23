#include "RenderScheme.h"

namespace ma
{
	RenderScheme::RenderScheme(Scene* pScene)
	{
		m_pScene = pScene;

		m_pGBufferPass = NULL;
		m_pDeferredShadowPass = NULL;
		m_pDeferredLightPass = NULL;
	}

	void RenderScheme::Init()
	{
		VEC_RENDERPASS::iterator it = m_arrRenderPass.begin();
		for (; it != m_arrRenderPass.end(); ++it)
		{
			RefPtr<RenderPass>& pRenderPass = *it;
			pRenderPass->Init();
		}	
	}

	void RenderScheme::ShoutDown()
	{
		VEC_RENDERPASS::iterator it = m_arrRenderPass.begin();
		for (; it != m_arrRenderPass.end(); ++it)
		{
			RefPtr<RenderPass>& pRenderPass = *it;
			pRenderPass->ShoutDown();
			pRenderPass = NULL;
		}	
		m_arrRenderPass.clear();
	}

	void RenderScheme::Render()
	{
		VEC_RENDERPASS::iterator it = m_arrRenderPass.begin();
		for (; it != m_arrRenderPass.end(); ++it)
		{
			RefPtr<RenderPass>& pRenderPass = *it;
			pRenderPass->Render();
		}	
	}

	void RenderScheme::AddRenderPass(RenderPass* pPass)
	{
		m_arrRenderPass.push_back(pPass);
	}

	void RenderScheme::AddGBufferPass()
	{
		m_pGBufferPass = new GBufferPass(m_pScene);
		AddRenderPass(m_pGBufferPass);
	}

	void RenderScheme::AddDeferredShadowPass()
	{	
		m_pDeferredShadowPass = new DeferredShadowPass(m_pScene);
		AddRenderPass(m_pDeferredShadowPass);
	}

	void RenderScheme::AddDeferredLightPass()
	{
		m_pDeferredLightPass = new DeferredLightPass(m_pScene);
		AddRenderPass(m_pDeferredLightPass);
	}

	void RenderScheme::AddShadingPass()
	{
		m_pShadingPass = new ShadingPass(m_pScene);
		AddRenderPass(m_pShadingPass);
	}

	RefPtr<RenderScheme> CreateRenderScheme(RenderScheme::Type eType, Scene* pScene)
	{
		RenderScheme* pRenderScheme = new RenderScheme(pScene);
		
		if (eType == RenderScheme::Forward)
		{
			pRenderScheme->AddShadingPass();
		}
		else if (eType == RenderScheme::DeferredLighting)
		{
			pRenderScheme->AddGBufferPass();
			pRenderScheme->AddDeferredShadowPass();
			pRenderScheme->AddDeferredLightPass();
			pRenderScheme->AddShadingPass();
		}
		
 		return pRenderScheme;
	}
}
