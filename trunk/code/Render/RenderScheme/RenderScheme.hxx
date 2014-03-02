#include "RenderScheme.h"

namespace ma
{
	RenderScheme::RenderScheme()
	{
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

}
