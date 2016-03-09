#include "BatchRenderable.h"


namespace ma
{

	struct SortDescendingLess
	{
		SortDescendingLess()
		{
		}

		bool operator()(const Renderable* a, const Renderable* b) const
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

	BatchRenderable::BatchRenderable()
	{

	}

	void BatchRenderable::AddRenderObj(Renderable* pRenderObj) 
	{
		m_arrRenderList.push_back(pRenderObj);
	}

	void BatchRenderable::PrepareRender()
	{
		std::sort(m_arrRenderList.begin(), m_arrRenderList.end(), SortDescendingLess());
	}

	void BatchRenderable::Render()
	{
		for (UINT i = 0; i < m_arrRenderList.size(); ++i)
		{
			Renderable* pRenderObj = m_arrRenderList[i];
			if (pRenderObj == NULL)
				continue; 

			Technique* pTech = pRenderObj->m_pSubMaterial->GetShadingTechnqiue();

			pRenderObj->Render(pTech);
		}
	}

	void BatchRenderable::Clear()
	{
		m_arrRenderList.clear();
	}

}

