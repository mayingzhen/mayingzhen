#include "RenderView.h"
#include "../RenderScheme/RenderStep.h"

namespace ma
{
	MainRenderView::MainRenderView()
	{
		m_pSceneproxy = std::make_shared<SceneContext>();

		m_nRenderOrder = -1;
	}

	MainRenderView::~MainRenderView()
	{

	}

	void MainRenderView::Update()
	{
		SceneContext* sceneData = new SceneContext();
		sceneData->SetCamera(m_pCamera.get());
		sceneData->SetMainLight(m_pScene->GetMainDirLight(), m_pScene->GetAmbientColor());
		GetRenderSystem()->RC_AddRenderCommad( [this,sceneData]()
			{
				*m_pSceneproxy = *sceneData;
				delete sceneData;
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


	void MainRenderView::AddRenderStep(MainRenderStep* step)
	{
		m_vecRenderStep.push_back(step); 
	}

	DeferredShadow* MainRenderView::GetDeferredShadowSetp() 
	{
		return m_pDeferredShadow.get();
	}

	void MainRenderView::SetDeferredShadowSetp(DeferredShadow* step)
	{
		m_pDeferredShadow = step;
	}

	void MainRenderView::Render()
	{
		ROFILE_SCOPEI("MainRenderView::Render", 0);

		GetRenderSystem()->BeginProfile("MainRenderView::Render");

		m_arrRenderProxy.clear();

		m_pScene->GetCullTree()->FindObjectsIn(&m_pSceneproxy->m_frustum, -1, m_arrRenderProxy);

		m_arrRenderProxy.insert(m_arrRenderProxy.end(), m_arrExternRenderProxy.begin(), m_arrExternRenderProxy.end());

		m_arrExternRenderProxy.clear();

		for (auto& step : m_vecRenderStep)
		{
			step->BeginePrepareRender(m_pSceneproxy.get());
		}

		if (GetJobScheduler()->GetNumThreads() > 0)
		{
			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(m_vecRenderStep.size());
			for (uint32_t iStep = 0; iStep < m_vecRenderStep.size(); ++iStep)
			{
				MainRenderStep* pStep = m_vecRenderStep[iStep].get();

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
			for (auto& step : m_vecRenderStep)
			{
				for (auto& render_porxy : m_arrRenderProxy)
				{
					step->PrepareRender(render_porxy);
				}
			}
		}

		for (auto& step : m_vecRenderStep)
		{
			step->Render(m_pSceneproxy.get());
		}

		GetRenderSystem()->EndProfile();
	}
}
