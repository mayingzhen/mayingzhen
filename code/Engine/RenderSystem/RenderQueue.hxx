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

	struct SortDescendingLess
	{
		SortDescendingLess()
		{
		}

		bool operator()(const RefPtr<Renderable> a, const RefPtr<Renderable> b) const
		{
			long i = long(a->GetMaterial()->GetShadingTechnqiue()->GetShaderProgram()) - long(b->GetMaterial()->GetShadingTechnqiue()->GetShaderProgram());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			i = long(a->GetMaterial()->GetShadingTechnqiue()) - long(b->GetMaterial()->GetShadingTechnqiue());
			if (i<0)
				return true;
			else if(i>0)
				return false;

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
		
		std::sort(arrRenderObj.begin(), arrRenderObj.end(), SortDescendingLess());
	}

	void RenderQueue::RenderObjList(RenderListType eRLType)
	{
		for (UINT i = 0; i < GetRenderObjNumber(eRLType); ++i)
		{
			Renderable* pRenderObj = GetRenderObjByIndex(eRLType,i);
			if (pRenderObj == NULL)
				continue; 

			Technique* pTech = pRenderObj->m_pSubMaterial->GetShadingTechnqiue();

			pRenderObj->Render(pTech);
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

