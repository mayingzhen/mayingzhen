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

	struct RenderObjListSortDescendingLess
	{
		RenderObjListSortDescendingLess()
		{
		}

		bool operator()(const Renderable* a, const Renderable* b) const
		{
			int i = 0;
			//int i = int(a->GetMaterial()->GetGpuProgram()->GetSortID()) - int(b->GetMaterial()->GetGpuProgram()->GetSortID());
			//if (i<0)
			//	return true;
			//else if(i>0)
			//	return false;

			i = int(a->GetMaterial()) - int(b->GetMaterial());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			return false;
		}
	};

	void RenderQueue::SortRenderObjList(RenderListType eRLType)
	{
		VEC_RENDERABLE& arrRenderObj = m_arrRenderList[eRLType];
		
		for (uint32 i = 0; i < arrRenderObj.size(); ++i)
		{

		}
	}

	void RenderQueue::Clear()
	{
		for (UINT i = 0; i < RL_Count; ++i)
		{
			m_arrRenderList[i].clear();
		}
	}
}

