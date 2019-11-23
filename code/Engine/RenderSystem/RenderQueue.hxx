#include "RenderQueue.h"
#include "BatchRenderable.h"
#include "RenderContext.h"


namespace ma
{
	RenderQueue::RenderQueue()	
	{
		m_renderContext = new RenderContext();
	}

	RenderQueue::~RenderQueue()
	{
	}

	void RenderQueue::AddRenderObj(int stage, Renderable* pRenderObj, Technique* pTech)
	{
		m_mapRenderList[stage].AddRenderObj(pRenderObj, pTech);
	}

	void RenderQueue::Render(RenderPass* pPass)
	{
		for (auto& it : m_mapRenderList)
		{
			Render(pPass, it.first, it.first);
		}
	}

	void RenderQueue::Render(RenderPass* pPass, int stageBegin, int stageEnd)
	{
		auto bi = m_mapRenderList.lower_bound(stageBegin);
		auto ei = m_mapRenderList.lower_bound(stageEnd);
		for (auto it = bi; it != ei; ++it)
		{
			it->second.PrepareRender();

			it->second.Render(pPass, it->first);
		}
	}

	void RenderQueue::Clear()
	{
		m_mapRenderList.clear();
	}

	void RenderQueue::SetCamera(Camera* pCamera)
	{
		m_matViewProj.SetMatView(pCamera->GetMatView());
		m_matViewProj.SetMatProj(pCamera->GetMatProj());
		m_vEyeWordPos = pCamera->GetPosWS();
		m_fFar = pCamera->GetFarClip();
		m_fNear = pCamera->GetNearClip();
	}

	void RenderQueue::AddLight(Light* pLight)
	{
		m_vecLight.push_back(pLight);
	}
}

