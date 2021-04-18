#include "RenderView.h"
#include "../RenderScheme/RenderStep.h"

namespace ma
{
	MainRenderView::MainRenderView()
	{
	}

	MainRenderView::~MainRenderView()
	{

	}

	void MainRenderView::Update()
	{
		SceneContext sceneData;
		sceneData.SetCamera(m_pCamera.get());
		GetRenderSystem()->RC_AddRenderCommad( [this,sceneData]()
			{
				*m_pSceneproxy = sceneData;
			}
		);
	}

	void MainRenderView::AddExternRenderProxy(RenderProxy* pRenderProxy)
	{
		GetRenderSystem()->RC_AddRenderCommad([this, pRenderProxy]()
			{
				m_arrExternRenderProxy.push_back(pRenderProxy);
			}
		);
	}

	void MainRenderView::Render()
	{
		MICROPROFILE_SCOPEI("", "MainRenderView::Render", 0);

		m_arrRenderProxy.clear();

		m_pScene->GetCullTree()->FindObjectsIn(&m_pSceneproxy->m_frustum, -1, m_arrRenderProxy);

		m_arrRenderProxy.insert(m_arrRenderProxy.end(), m_arrExternRenderProxy.begin(), m_arrExternRenderProxy.end());

		m_arrExternRenderProxy.clear();

		for (uint32_t iStep = 0; iStep < m_vecRenderStep.size(); ++iStep)
		{
			RenderStep* pStep = m_vecRenderStep[iStep].get();
			pStep->BeginePrepareRender();
		}

		if (GetJobScheduler()->GetNumThreads() > 0)
		{
			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(m_vecRenderStep.size());
			for (uint32_t iStep = 0; iStep < m_vecRenderStep.size(); ++iStep)
			{
				RenderStep* pStep = m_vecRenderStep[iStep].get();

				GetJobScheduler()->SubmitJob(jobGroup,
					[pStep, this]() {
						for (uint32_t iProxy = 0; iProxy < m_arrRenderProxy.size(); ++iProxy)
						{
							pStep->PrepareRender(m_arrRenderProxy[iProxy]);
						}
					}
				);
			}
			GetJobScheduler()->WaitForGroup(jobGroup);
		}
		else
		{
			for (uint32_t iStep = 0; iStep < m_vecRenderStep.size(); ++iStep)
			{
				RenderStep* pStep = m_vecRenderStep[iStep].get();

				for (uint32_t iProxy = 0; iProxy < m_arrRenderProxy.size(); ++iProxy)
				{
					pStep->PrepareRender(m_arrRenderProxy[iProxy]);
				}
			}
		}

		for (auto& step : m_vecRenderStep)
		{
			step->Render();
		}
	}
}
