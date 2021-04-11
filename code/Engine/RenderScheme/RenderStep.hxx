#include "RenderScheme.h"
#include "../RenderSystem/RenderCommand.h"
#include "../RenderSystem/RenderProfile.h"
#include "PostProcess.h"

namespace ma
{

	RenderStep::RenderStep()
	{
		m_pRenderQueue = new RenderQueue;
	}
	

	RenderStep::~RenderStep()
	{

	}

	RefPtr<RenderStep> CreateRenderStep()
	{
		return new RenderStep();
	}


	GbufferStep::GbufferStep()
	{
		m_strName == "GBuffer";

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
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("tDeviceDepthMapSampler", [this](Renderable*)->SamplerState* {
			return m_pDepthSampler.get();
			});

		m_pBaseSampler = CreateSamplerState(m_pBaseColor.get(), CLAMP, TFO_POINT, false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_textureSceneDiffuse", [this](Renderable*)->SamplerState* {
			return m_pBaseSampler.get();
			});

		m_pNormalSampler = CreateSamplerState(m_pNormalTex.get(), CLAMP, TFO_POINT, false);
		GetParameterManager()->AddFunMethodBinding<SamplerState*>("u_textureSceneNormal", [this](Renderable*)->SamplerState* {
			return m_pNormalSampler.get();
			});
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

			Technique* tech = renderable->GetMaterial()->GetTechnqiue(m_pRenderPass.get());
			if (tech == nullptr || tech->GetTransluce())
			{
				continue;
			}

			m_pRenderQueue->AddRenderObj(proxy->GetRenderOrder(), renderable, tech);
		}
	}

	void GbufferStep::Render()
	{
		m_pRenderPass->Begine();

		m_pRenderQueue->Render(m_pRenderPass.get());

		m_pRenderPass->End();
	}

	TransluceStep::TransluceStep()
	{
		m_pRenderPass = GetRenderSystem()->GetBackBufferRenderPass();
	}

	void TransluceStep::PrepareRender(RenderProxy* proxy)
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

			Technique* tech = renderable->GetMaterial()->GetTechnqiue(m_pRenderPass.get());
			if (tech  == nullptr || !tech->GetTransluce())
			{
				continue;
			}

			m_pRenderQueue->AddRenderObj(proxy->GetRenderOrder(), renderable, tech);
		}
	}

	void TransluceStep::Render()
	{
		m_pRenderQueue->Render(m_pRenderPass.get());
	}


	DefferedLightStep::DefferedLightStep()
	{
		m_pHDRColorTex = GetRenderSystem()->CreateRenderTarget(-1, -1, 1, PF_FLOAT16_RGBA);

		m_pRenderPass = GetRenderDevice()->CreateRenderPass();
		m_pRenderPass->AttachColor(0, RenderSurface(m_pHDRColorTex));
		//RenderSurface depth(m_pDepthTex);
		//depth.m_eLoadOp = LOAD_OP_LOAD;
		//m_pRenderPass->AttachDepthStencil(depth);
		GetRenderSystem()->RenderPassStreamComplete(m_pRenderPass.get());

		GetRenderSystem()->SetDefferedLightRenderPass(m_pRenderPass.get());
	}


	void DefferedLightStep::PrepareRender(RenderProxy* proxy)
	{
		LightProxy* light = dynamic_cast<LightProxy*>(proxy);
		if (light == nullptr)
		{
			return;
		}

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

				m_pRenderQueue->AddRenderObj(RL_Mesh, ScreenQuad::GetRenderable(), light->m_pTech.get());
			}
			else if (light->m_eLightType == LIGHT_POINT)
			{
				PointLightProxy* pPointLight = dynamic_cast<PointLightProxy*>(light);
				ASSERT(pPointLight);

				Uniform* pUniformPosRadius = light->m_pTech->GetUniform(PS, "light_pos_radius");
				light->m_pTech->SetValue(pUniformPosRadius, pPointLight->m_vPosRadius[index]);

				m_pRenderQueue->AddRenderObj(RL_Mesh, pPointLight->m_pSphere.get(), light->m_pTech.get());
			}
		}
	}

	void DefferedLightStep::Render()
	{
		m_pRenderQueue->Render(m_pRenderPass.get());
	}
}


