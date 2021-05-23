#include "RenderScheme.h"
#include "../RenderSystem/RenderCommand.h"
#include "../RenderSystem/RenderProfile.h"
#include "PostProcess.h"

namespace ma
{

// 	RenderStep::RenderStep()
// 	{
// 		
// 	}
// 	
// 
// 	RenderStep::~RenderStep()
// 	{
// 
// 	}

	MainRenderStep::MainRenderStep()
	{
		m_pRenderQueue = new RenderQueue;
	}

	MainRenderStep::~MainRenderStep()
	{

	}

	void MainRenderStep::BeginePrepareRender()
	{
		m_pRenderQueue->Clear();
	}

	void MainRenderStep::Render(SceneContext* sc)
	{
		GetRenderSystem()->BeginProfile(m_strName.c_str());

		m_pRenderPass->Begine();

		m_pRenderQueue->Render(m_pRenderPass.get(), sc);

		m_pRenderPass->End();

		GetRenderSystem()->EndProfile();
	}

	GbufferStep::GbufferStep()
	{
		m_strName = "GBuffer";

		m_pDepthTex = GetRenderSystem()->CreateDepthStencil(-1, -1, PF_D24S8);
		m_pBaseColor = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8, false);
		m_pNormalTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_A8R8G8B8, false);

		m_pRenderPass = GetRenderDevice()->CreateRenderPass();
		m_pRenderPass->AttachDepthStencil(RenderSurface(m_pDepthTex));
		m_pRenderPass->AttachColor(0, RenderSurface(m_pBaseColor));
		m_pRenderPass->AttachColor(1, RenderSurface(m_pNormalTex));
		GetRenderSystem()->RenderPassStreamComplete(m_pRenderPass.get());

		GetRenderSystem()->SetBaseRenderPass(m_pRenderPass.get());

		m_pDepthSampler = CreateSamplerState(m_pDepthTex.get(), CLAMP, TFO_POINT, false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("tDeviceDepthMapSampler", [this](Renderable*,SceneContext*)->SamplerState* {
			return m_pDepthSampler.get();
			});

		m_pBaseSampler = CreateSamplerState(m_pBaseColor.get(), CLAMP, TFO_POINT, false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_textureSceneDiffuse", [this](Renderable*, SceneContext*)->SamplerState* {
			return m_pBaseSampler.get();
			});

		m_pNormalSampler = CreateSamplerState(m_pNormalTex.get(), CLAMP, TFO_POINT, false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_textureSceneNormal", [this](Renderable*, SceneContext*)->SamplerState* {
			return m_pNormalSampler.get();
			});
	}

	void GbufferStep::BeginePrepareRender()
	{
		MainRenderStep::BeginePrepareRender();

		m_batchRender.Clear();
	}

	void GbufferStep::PrepareRender(RenderProxy* proxy)
	{
		if (proxy == nullptr)
		{
			return;
		}

		for (uint32_t i = 0; i < proxy->GetRenderableCount(); ++i)
		{
			Renderable* renderable = proxy->GetRenderableByIndex(i);
			if (renderable == nullptr)
			{
				continue;
			}

			if (renderable->GetRenderOrder() > RL_SkyBox)
			{
				continue;
			}

			Technique* tech = renderable->GetMaterial()->GetTechnqiue(m_pRenderPass.get());
			if (tech == nullptr || tech->GetTransluce())
			{
				continue;
			}

			if (renderable->m_bSuportInstace)
			{
				m_batchRender.AddRenderObj(renderable, tech);
			}
			else
			{
				m_pRenderQueue->AddRenderObj(renderable, tech);
			}
		}

		m_batchRender.PrepareRender(m_pRenderQueue.get());
	}

	DefferedLightStep::DefferedLightStep(Texture* pDepthTexture)
	{
		m_strName = "DefferedLightStep";

		m_pHDRColorTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_R11G11B10_FLOAT);

		m_pRenderPass = GetRenderDevice()->CreateRenderPass();
		RenderSurface color(m_pHDRColorTex);
		color.m_cClearColor = ColourValue::Black;
		m_pRenderPass->AttachColor(0, color);
		RenderSurface depth(pDepthTexture);
		depth.m_eLoadOp = LOAD_OP_LOAD;
		//m_pRenderPass->AttachDepthStencil(depth);
		GetRenderSystem()->RenderPassStreamComplete(m_pRenderPass.get());

		GetRenderSystem()->SetDefferedLightRenderPass(m_pRenderPass.get());
	}

	void DefferedLightStep::PrepareLightProxy(LightProxy* light)
	{
		uint32_t index = GetRenderSystem()->CurThreadProcess();

		{
			Uniform* pUniformColor = light->m_pTech->GetUniform(PS, "light_color");
			light->m_pTech->SetValue(pUniformColor, light->m_vLightColor[index]);

			if (light->m_eLightType == LIGHT_DIRECTIONAL)
			{
				DirLightProxy* pDirLight = dynamic_cast<DirLightProxy*>(light);
				ASSERT(pDirLight);

				Uniform* pUniformDir = light->m_pTech->GetUniform(PS, "light_dir");
				light->m_pTech->SetValue(pUniformDir, pDirLight->m_vDir[index]);

				m_pRenderQueue->AddRenderObj(pDirLight->m_pQuad.get(), light->m_pTech.get());
			}
			else if (light->m_eLightType == LIGHT_POINT)
			{
				PointLightProxy* pPointLight = dynamic_cast<PointLightProxy*>(light);
				ASSERT(pPointLight);

				Uniform* pUniformPosRadius = light->m_pTech->GetUniform(PS, "light_pos_radius");
				light->m_pTech->SetValue(pUniformPosRadius, pPointLight->m_vPosRadius[index]);

				m_pRenderQueue->AddRenderObj(pPointLight->m_pSphere.get(), light->m_pTech.get());
			}
		}
	}

	void DefferedLightStep::PrepareRender(RenderProxy* proxy)
	{
		LightProxy* light = dynamic_cast<LightProxy*>(proxy);
		if (light)
		{
			PrepareLightProxy(light);
		}
		else
		{
			for (uint32_t i = 0; i < proxy->GetRenderableCount(); ++i)
			{
				Renderable* renderable = proxy->GetRenderableByIndex(i);
				if (renderable->GetRenderOrder() > RL_Transluce && renderable->GetRenderOrder() < RL_PostProcess)
				{
					Technique* tech = renderable->GetMaterial()->GetTechnqiue(m_pRenderPass.get());
					m_pRenderQueue->AddRenderObj(renderable, tech);
				}
			}
		}
	}

	UIStep::UIStep(SubMaterial* pLastPostProcss)
	{
		m_strName = "UIStep";

		m_pLastPostProcss = pLastPostProcss;

		m_pRenderPass = GetRenderSystem()->GetBackBufferRenderPass();
	}

	void UIStep::BeginePrepareRender()
	{
		MainRenderStep::BeginePrepareRender();

		if (m_pLastPostProcss)
		{
			ScreenQuad::GetRenderable()->m_nRenderOrder = 0;

			m_pRenderQueue->AddRenderObj(ScreenQuad::GetRenderable(), m_pLastPostProcss->GetShadingTechnqiue());
		}
	}

	void UIStep::PrepareRender(RenderProxy* proxy)
	{
		if (proxy == nullptr)
		{
			return;
		}

		for (uint32_t i = 0; i < proxy->GetRenderableCount(); ++i)
		{
			Renderable* renderable = proxy->GetRenderableByIndex(i);
			if (renderable == nullptr || renderable->GetRenderOrder() != RL_UI)
			{
				continue;
			}

			Technique* tech = renderable->GetMaterial()->GetTechnqiue(m_pRenderPass.get());
			m_pRenderQueue->AddRenderObj(renderable, tech);
		}
	}
}


