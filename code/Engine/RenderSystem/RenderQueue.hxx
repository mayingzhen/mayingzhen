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
		int index = GetRenderThread()->m_nCurThreadFill;

		std::vector<RenderObject*>& arrRenderObj = m_arrRenderList[index][eRLType];
		
		arrRenderObj.push_back(pRenderObj);
	}

	UINT RenderQueue::GetRenderObjNumber(RenderListType eRLType)
	{
		int index = GetRenderThread()->m_nCurThreadProcess;

		std::vector<RenderObject*>& arrRenderObj = m_arrRenderList[index][eRLType];

		return arrRenderObj.size();
	}

	RenderObject* RenderQueue::GetRenderObjByIndex(RenderListType eRLType,int index)
	{
		int nThreadindex = GetRenderThread()->m_nCurThreadProcess;

		std::vector<RenderObject*>& arrRenderObj = m_arrRenderList[nThreadindex][eRLType];

		return arrRenderObj[index];
	}

	void RenderQueue::Clear()
	{
		int index = GetRenderThread()->CurThreadFill();
		for (UINT i = 0; i < RL_Count; ++i)
		{
			m_arrRenderList[index][i].clear();	
		}
	}
}

