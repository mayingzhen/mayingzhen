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
		for (int stage = stageBegin; stage <= stageEnd; ++stage)
		{
			auto it = m_mapRenderList.find(stage);
			if (it == m_mapRenderList.end())
			{
				continue;
			}

			it->second.PrepareRender();

			it->second.Render(pPass, it->first);
		}
	}

	void RenderQueue::Render(RenderCommand* pRenderCommand, int stageBegin, int stageEnd)
	{
		for (int stage = stageBegin; stage <= stageEnd; ++stage)
		{
			auto it = m_mapRenderList.find(stage);
			if (it == m_mapRenderList.end())
			{
				continue;
			}

			it->second.PrepareRender();

			it->second.Render(pRenderCommand, it->first);
		}
	}

	void RenderQueue::Render(RenderCommand* pRenderCommand)
	{
		for (auto& it : m_mapRenderList)
		{
			Render(pRenderCommand, it.first, it.first);
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

	void RenderQueue::SetLightViewProj(const Matrix4& matLightViewProj)
	{
		if (m_renderContext)
		{
			m_renderContext->SetLightViewProj(matLightViewProj);
		}
	}

	void RenderQueue::SetMainLight(Light* pMainLight, ColourValue cAmbient)
	{
		if (m_renderContext)
		{
			m_renderContext->SetMainLight(pMainLight, cAmbient);
		}
	}

	void RenderQueue::AddLightObj(LightProxy* pLight)
	{
		m_vecLight.push_back(pLight);
	}
}

