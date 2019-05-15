#include "RenderQueue.h"
#include "BatchRenderable.h"


namespace ma
{
	RenderQueue::RenderQueue()	
	{
	}

	RenderQueue::~RenderQueue()
	{
	}

	void RenderQueue::AddRenderObj(int stage, Renderable* pRenderObj, Technique* pTech)
	{
		m_mapRenderList[stage].AddRenderObj(pRenderObj, pTech);
	}

	void RenderQueue::Render(RenderPass* pPass, int stageBegin, int stageEnd)
	{
		auto bi = m_mapRenderList.lower_bound(stageBegin);
		auto ei = m_mapRenderList.lower_bound(stageEnd);
		for (auto it = bi; it != ei; ++it)
		{
			it->second.PrepareRender();

			it->second.Render(pPass, it->first);
		}
	}

	void RenderQueue::Clear()
	{
		m_mapRenderList.clear();
	}
}

