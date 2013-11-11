#include "RenderQueue.h"


namespace ma
{
	static RenderQueue* gpRenderQueue = NULL;

	RenderQueue* GetRenderQueue()
	{
		return gpRenderQueue;
	}

	void SetRenderQueue(RenderQueue* pRenderQueue)
	{
		gpRenderQueue = pRenderQueue;
	}

	void RenderQueue::AddRenderObj(RenderListType eRLType,RenderObject* pRenderObj)
	{
		std::vector<RenderObject*>& arrRenderObj = m_arrRenderList[eRLType];
		
		arrRenderObj.push_back(pRenderObj);
	}

	UINT RenderQueue::GetRenderObjNumber(RenderListType eRLType)
	{
		std::vector<RenderObject*>& arrRenderObj = m_arrRenderList[eRLType];

		return arrRenderObj.size();
	}

	RenderObject* RenderQueue::GetRenderObjByIndex(RenderListType eRLType,int index)
	{
		std::vector<RenderObject*>& arrRenderObj = m_arrRenderList[eRLType];

		return arrRenderObj[index];
	}

	void RenderQueue::Clear()
	{
		for (UINT i = 0; i < RL_Count; ++i)
		{
			m_arrRenderList[i].clear();	
		}
	}
}

