#include "ShadowRenderView.h"
#include "../RenderScheme/RenderStep.h"

namespace ma
{
	ShadowRenderView::ShadowRenderView()
	{
		m_pSceneproxy = std::make_shared<SceneContext>();
	}

	void ShadowRenderView::AddRenderStep(ShadowRenderStep* step)
	{
		GetRenderSystem()->RC_AddRenderCommad([this, step]() {
			m_vecRenderStep.push_back(step);
			});
	}

	void ShadowRenderView::Update()
	{
		SceneContext sceneData;
		sceneData.SetCamera(m_pCamera.get());
		GetRenderSystem()->RC_AddRenderCommad([this, sceneData]()
			{
				*m_pSceneproxy = sceneData;
			}
		);

	}

	void ShadowRenderView::Render()
	{
		GetRenderSystem()->BeginProfile("ShadowMapRenderView::Render");

		for (auto& subStep : m_vecRenderStep)
		{
			subStep->PrepareRender();
		}

		m_pRenderPass->Begine();

		uint32_t i = 0;
		for (auto& subStep : m_vecRenderStep)
		{
			RenderCommand* pRenderCommand = m_pRenderPass->GetThreadCommand(i++);

			pRenderCommand->Begin();

			RenderQueue* cur_renderQueue = subStep->m_pRenderQueue.get();

			pRenderCommand->SetViewPort(subStep->m_veiwPort);

			cur_renderQueue->Render(pRenderCommand, m_pSceneproxy.get());

			pRenderCommand->End();
		}

		m_pRenderPass->End();

		GetRenderSystem()->EndProfile();
	}
}
