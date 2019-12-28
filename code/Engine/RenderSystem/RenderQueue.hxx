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
		if (stageBegin == stageEnd)
		{
			auto it = m_mapRenderList.lower_bound(stageBegin);

			it->second.PrepareRender();

			it->second.Render(pPass, it->first);
		}
		else
		{
			auto bi = m_mapRenderList.lower_bound(stageBegin);
			auto ei = m_mapRenderList.lower_bound(stageEnd);
			for (auto it = bi; it != ei; ++it)
			{
				it->second.PrepareRender();

				it->second.Render(pPass, it->first);
			}
		}
	}

	void RenderQueue::Clear()
	{
		m_mapRenderList.clear();
		m_vecLight.clear();
	}

	void RenderQueue::SetCamera(Camera* pCamera)
	{
		if (m_renderContext)
		{
			m_renderContext->SetCamera(pCamera);
		}
	}

	void RenderQueue::AddLight(Light* pLight)
	{
		m_vecLight.emplace_back();
		LightInfo& info = m_vecLight.back();
		info.m_eType = pLight->GetLightType();
		info.m_cLightColor = pLight->GetLightColor();
		info.m_fLightIntensity = pLight->GetLightIntensity();
		info.m_vDir = pLight->GetSceneNode()->GetForward();
		info.m_vPos = pLight->GetSceneNode()->GetPosWS();
	}
}

