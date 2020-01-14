#include "RenderQueue.h"
#include "BatchRenderable.h"
#include "RenderContext.h"


namespace ma
{
	RenderQueue::RenderQueue()	
	{
		m_renderContext = new SceneContext();
	}

	RenderQueue::~RenderQueue()
	{
	}

	void RenderQueue::AddRenderObj(int stage, Renderable* pRenderObj, Technique* pTech)
	{
		m_mapRenderList[stage].AddRenderObj(pRenderObj, pTech);
	}

	void RenderQueue::AddComputeObj(Renderable* pRenderObj, Technique* pTech)
	{
		m_compute.AddRenderObj(pRenderObj, pTech);
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
			if (it == m_mapRenderList.end())
			{
				return;
			}

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

	void RenderQueue::Compute()
	{
		ComputeCommad* pCommand = GetRenderSystem()->GetComputeCommad();

		//pCommand->Begin();
		GetRenderSystem()->BegineCompute();

		m_compute.Compute(GetRenderSystem()->GetComputeCommad());

		//pCommand->End();
		GetRenderSystem()->EndCompute();
	}

	void RenderQueue::Clear()
	{
		m_mapRenderList.clear();
		m_vecLight.clear();
		m_compute.Clear();
	}

	void RenderQueue::SetCamera(Camera* pCamera)
	{
		if (m_renderContext)
		{
			m_renderContext->SetCamera(pCamera);
		}
	}

	void RenderQueue::SetMainLight(Light* pMainLight, ColourValue cAmbient)
	{
		if (m_renderContext)
		{
			m_renderContext->SetMainLight(pMainLight, cAmbient);
		}
	}

	void RenderQueue::AddLight(Light* pLight, Technique* pTech)
	{
		m_vecLight.emplace_back();
		LightInfo& info = m_vecLight.back();
		info.m_eType = pLight->GetLightType();
		info.m_cLightColor = pLight->GetLightColor();
		info.m_fLightIntensity = pLight->GetLightIntensity();
		info.m_vDir = pLight->GetSceneNode()->GetForward();
		info.m_vPos = pLight->GetSceneNode()->GetPosWS();
		info.m_pTech = pTech;
		if (pLight->GetLightType() == LIGHT_POINT)
		{
			PointLight* pPointLight = (PointLight*)(pLight);
			info.m_fRadius = pPointLight->GetRadius();
		}
	}
}

