#include "RenderScheme.h"

namespace ma
{
	RenderScheme::RenderScheme()
	{
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

	RefPtr<RenderScheme> CreateRenderScheme(RenderScheme::Type eType)
	{
 		RenderScheme* pRenderScheme = new RenderScheme();
		
		if (eType == RenderScheme::Forward)
		{
			pRenderScheme->AddRenderPass( new ShadowDepthPass() );
			pRenderScheme->AddRenderPass( new ShadingPass() );
		}
		else if (eType == RenderScheme::DeferredLighting)
		{
	 		pRenderScheme->AddRenderPass( new ShadowDepthPass() );
	 		pRenderScheme->AddRenderPass( new GBufferPass() );
	 		pRenderScheme->AddRenderPass( new DeferredShadowPass() );
	 		pRenderScheme->AddRenderPass( new DeferredLightPass() );
	 		pRenderScheme->AddRenderPass( new ShadingPass() );	
		}
		
 		return pRenderScheme;
	}
}
