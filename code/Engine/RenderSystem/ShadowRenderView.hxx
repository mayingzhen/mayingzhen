#include "ShadowRenderView.h"
#include "../RenderScheme/RenderStep.h"
#include "../RenderScheme/DeferredShadowPass.h"

namespace ma
{
	ShadowRenderView::ShadowRenderView()
	{
		m_pSceneproxy = std::make_shared<SceneContext>();

		m_nRenderOrder = 0;
	}

	void ShadowRenderView::AddRenderStep(ShadowRenderStep* step)
	{
		GetRenderSystem()->RC_AddRenderCommad([this, step]() {
			m_vecRenderStep.push_back(step);
			});
	}

	void ShadowRenderView::Update()
	{
		SceneContext* sceneData = new SceneContext();
		sceneData->SetCamera(m_pCamera.get());
		GetRenderSystem()->RC_AddRenderCommad([this, sceneData]()
			{
				*m_pSceneproxy = *sceneData;
				delete sceneData;
			}
		);

	}


	void ShadowRenderView::PrepareDeferredShadow(ShadowRenderStep* step)
	{
		SMFrustumInfo info;
		
		Vector4 vNear(0, 0, step->GetSplitNear(), 1.0);
		Vector4 vFar(0, 0, step->GetSplitFar(), 1.0);
		vNear = m_pSceneproxy->m_matViewProj.GetMatProj() * vNear;
		vFar = m_pSceneproxy->m_matViewProj.GetMatProj() * vFar;
		info.m_fDepthNear = vNear.z / vNear.w;
		info.m_fDepthFar = vFar.z / vFar.w;

		ASSERT(info.m_fDepthNear >= 0.0 && info.m_fDepthNear < 1.0);
		ASSERT(info.m_fDepthFar >= 0.0 && info.m_fDepthFar < 1.0);
		ASSERT(info.m_fDepthNear < info.m_fDepthFar);

		info.m_matViewToShadow = step->GetShadowMatrix() * m_pSceneproxy->m_matViewProj.GetMatViewInv();
		info.m_pShadowDepth = m_pShadowMapSampler.get();

		DeferredShadow* pDeferredShadow = GetRenderSystem()->GetMainRenderView()->GetDeferredShadowSetp();
		if (pDeferredShadow)
		{
			GetRenderSystem()->GetMainRenderView()->GetDeferredShadowSetp()->AddSMFrustumInfo(info);
		}
	}

	void ShadowRenderView::Render()
	{
		GetRenderSystem()->BeginProfile("ShadowMapRenderView::Render");

		for (auto& subStep : m_vecRenderStep)
		{
			subStep->PrepareRender(m_pSceneproxy.get(), m_pScene->GetCullTree());

			PrepareDeferredShadow(subStep.get());
		}

		m_pRenderPass->Begine();

		uint32_t i = 0;
		for (auto& subStep : m_vecRenderStep)
		{
			RenderCommand* pRenderCommand = m_pRenderPass->GetThreadCommand(i++);

			pRenderCommand->Begin();

			RenderQueue* cur_renderQueue = subStep->m_pRenderQueue.get();

			pRenderCommand->SetViewPort(subStep->m_veiwPort);

			m_pSceneproxy->m_matLightViewProj = subStep->GetLightViewProjMatrix();

			cur_renderQueue->Render(pRenderCommand, m_pSceneproxy.get());

			pRenderCommand->End();
		}

		m_pRenderPass->End();

		GetRenderSystem()->EndProfile();
	}
}
