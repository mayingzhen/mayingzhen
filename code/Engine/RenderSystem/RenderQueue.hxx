#include "RenderQueue.h"
#include "BatchRenderable.h"
#include "BatchTerrainRenderable.h"


namespace ma
{
	RenderQueue::RenderQueue()	
	{
		m_arrRenderList[RL_Mesh] = new BatchRenderable();
		m_arrRenderList[RL_MeshTrans] = new BatchRenderable();
		m_arrRenderList[RL_Terrain] = new BatchTerrainRenderable();
	}

	RenderQueue::~RenderQueue()
	{
		for (UINT i = 0; i < RL_Count; ++i)
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
		for (UINT i = 0; i < RL_Count; ++i)
		{
			m_arrRenderList[i]->Clear();
		}
	}
}

