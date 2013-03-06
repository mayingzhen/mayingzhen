#include "RenderQueue.h"

namespace ma
{
	RenderQueue* gpRenderQueue = NULL;
	
	RenderQueue* GetRenderQueue()
	{
		return gpRenderQueue;
	}

	void SetRenderQueue(RenderQueue* pRenderQueue)
	{
		gpRenderQueue = pRenderQueue;
	}

	RenderQueue::RenderQueue()
	{

	}

	RenderQueue::~RenderQueue()
	{

	}

	void RenderQueue::AddRenderer(IRenderItem * obj)
	{
		if (obj == NULL)
			return;

		m_SolidEntry.push_back(obj);
	}

	void RenderQueue::Clear()
	{
		m_SolidEntry.clear();
		m_TransEntry.clear();
	}

	void RenderQueue::Flush()
	{
		
	}
}