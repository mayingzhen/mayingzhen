#include "RenderQueue.h"


namespace ma
{
	void RenderQueue::AddRenderObj(RenderListType eRLType,Renderable* pRenderObj)
	{
		VEC_RENDERABLE& arrRenderObj = m_arrRenderList[eRLType];
		arrRenderObj.push_back(pRenderObj);
	}

	UINT RenderQueue::GetRenderObjNumber(RenderListType eRLType)
	{
		VEC_RENDERABLE& arrRenderObj = m_arrRenderList[eRLType];

		return arrRenderObj.size();
	}

	Renderable* RenderQueue::GetRenderObjByIndex(RenderListType eRLType,int index)
	{
		VEC_RENDERABLE& arrRenderObj = m_arrRenderList[eRLType];

		return arrRenderObj[index].get();
	}

	void RenderQueue::Clear()
	{
		for (UINT i = 0; i < RL_Count; ++i)
		{
			m_arrRenderList[i].clear();
		}
	}
}

