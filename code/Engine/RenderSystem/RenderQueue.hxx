#include "RenderQueue.h"
#include "BatchRenderable.h"


namespace ma
{
	RenderQueue::RenderQueue()	
	{
		for (size_t i = 0; i < RL_Count; i++)
		{
			m_arrRenderList[i] = new BatchRenderable();
		}
	}

	RenderQueue::~RenderQueue()
	{
		for (uint32_t i = 0; i < RL_Count; ++i)
		{
			SAFE_DELETE(m_arrRenderList[i]);
		}
	}

	void RenderQueue::AddRenderObj(RenderListType eRLType,Renderable* pRenderObj)
	{
		m_arrRenderList[eRLType]->AddRenderObj(pRenderObj);
	}

	void RenderQueue::RenderObjList(RenderPass* pPass, RenderListType eRLType, RenderPassType eRPType)
	{
		m_arrRenderList[eRLType]->PrepareRender(eRPType);
		m_arrRenderList[eRLType]->Render(pPass,eRPType, eRLType);
	}

	void RenderQueue::Clear()
	{
		for (uint32_t i = 0; i < RL_Count; ++i)
		{
			m_arrRenderList[i]->Clear();
		}
	}
}

