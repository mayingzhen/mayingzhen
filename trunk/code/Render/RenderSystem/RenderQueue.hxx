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

	void RenderQueue::AddRenderObj(RenderListType eRLType,RenderComponent* pRenderObj)
	{
		std::vector<RenderComponent*>& arrRenderObj = m_arrRenderList[eRLType];
		arrRenderObj.push_back(pRenderObj);

		std::vector<GameObjectPtr>& arrGameObj = m_arrGameObject[eRLType];
		arrGameObj.push_back(pRenderObj->GetGameObject());
	}

	UINT RenderQueue::GetRenderObjNumber(RenderListType eRLType)
	{
		std::vector<RenderComponent*>& arrRenderObj = m_arrRenderList[eRLType];

		return arrRenderObj.size();
	}

	RenderComponent* RenderQueue::GetRenderObjByIndex(RenderListType eRLType,int index)
	{
		std::vector<RenderComponent*>& arrRenderObj = m_arrRenderList[eRLType];

		return arrRenderObj[index];
	}

	void RenderQueue::Clear()
	{
		for (UINT i = 0; i < RL_Count; ++i)
		{
			m_arrRenderList[i].clear();
			m_arrGameObject[i].clear();
		}
	}
}

