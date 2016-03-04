#include "RenderQueue.h"
#include "BatchRenderable.h"
#include "BatchParticleRenderable.h"
#include "BatchTerrainRenderable.h"


namespace ma
{
	RenderQueue::RenderQueue()	
	{
		m_arrRenderList[RL_Mesh] = new BatchRenderable();
		m_arrRenderList[RL_MeshTrans] = new BatchRenderable();
		m_arrRenderList[RL_Particle] = new BatchParticleRenderable();
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

	void RenderQueue::RenderObjList(RenderListType eRLType)
	{
		m_arrRenderList[eRLType]->PrepareRender();
		m_arrRenderList[eRLType]->Render();
	}

	void RenderQueue::Clear()
	{
		for (UINT i = 0; i < RL_Count; ++i)
		{
			m_arrRenderList[i]->Clear();
		}
	}
}

