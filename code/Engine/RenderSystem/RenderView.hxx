#include "RenderView.h"
#include "../RenderScheme/RenderStep.h"

namespace ma
{
	LightProcess::LightProcess(/*RenderStep* plightStep*/)
	{
		//m_pLightStep = plightStep;

		m_pAmbientLight = CreateTechnique("shader/ambientLight.tech", "", m_pLightStep->m_pRenderPass.get());
	}

	void LightProcess::Render()
	{
		RenderQueue* pRenderQueue = m_pLightStep->m_pRenderQueue.get();

		Vector3 cAmbientColor = pRenderQueue->GetSceneContext()->GetAmbientColor();
		m_pAmbientLight->SetParameter("light_color", Any(cAmbientColor));
		pRenderQueue->AddRenderObj(RL_Mesh, ScreenQuad::GetRenderable(), m_pAmbientLight.get());

		uint32_t index = GetRenderSystem()->CurThreadProcess();

		for (auto& light : m_vecLight)
		{
			Uniform* pUniformColor = light->m_pTech->GetUniform(PS, "light_color");
			light->m_pTech->SetValue(pUniformColor, light->m_vLightColor[index]);

			if (light->m_eLightType == LIGHT_DIRECTIONAL)
			{
				DirLightProxy* pDirLight = dynamic_cast<DirLightProxy*>(light.get());
				ASSERT(pDirLight);

				Uniform* pUniformDir = light->m_pTech->GetUniform(PS, "light_dir");
				light->m_pTech->SetValue(pUniformDir, pDirLight->m_vDir[index]);

				pRenderQueue->AddRenderObj(RL_Mesh, ScreenQuad::GetRenderable(), light->m_pTech.get());
			}
			else if (light->m_eLightType == LIGHT_POINT)
			{
				PointLightProxy* pPointLight = dynamic_cast<PointLightProxy*>(light.get());
				ASSERT(pPointLight);

				Uniform* pUniformPosRadius = light->m_pTech->GetUniform(PS, "light_pos_radius");
				light->m_pTech->SetValue(pUniformPosRadius, pPointLight->m_vPosRadius[index]);

				pRenderQueue->AddRenderObj(RL_Mesh, pPointLight->m_pSphere.get(), light->m_pTech.get());
			}
		}
	}

	MainRenderView::MainRenderView()
	{

	}

	MainRenderView::~MainRenderView()
	{

	}

	void MainRenderView::Update()
	{

	}

	void MainRenderView::Render()
	{
		MICROPROFILE_SCOPEI("", "MainRenderView::Render", 0);

		m_arrRenderProxy.clear();

		m_pScene->GetCullTree()->FindObjectsIn(&m_pCamera->GetFrustum(), -1, m_arrRenderProxy);

		for (auto& step : m_vecRenderStep)
		{
			for (uint32_t i = 0; i < m_arrRenderProxy.size(); ++i)
			{
				step->PrepareRender(m_arrRenderProxy[i]);
			}
		}

		for (auto& step : m_vecRenderStep)
		{
			step->Render();
		}
	}
}
