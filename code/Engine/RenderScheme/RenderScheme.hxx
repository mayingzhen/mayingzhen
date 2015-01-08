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

	void RenderScheme::Serialize(Serializer& sl, const char* pszLable/* = "RenderScheme"*/)
	{
		sl.BeginSection(pszLable);

		//SerializeArrObj<RenderPass>(sl,m_arrRenderPass,"arrAnimNode");

		sl.EndSection();
	}

	RefPtr<RenderScheme> CreateRenderScheme()
	{
		return new RenderScheme();
	}
}
