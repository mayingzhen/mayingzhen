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

	void RenderStep::Render()
	{

	}

	void RenderStep::Render(RenderPass* prePass,RenderPass* nextPass)
	{
		RenderQueue* cur_renderQueue = m_pRenderQueue.get();
		RenderPass*  cur_renderPass = m_pRenderPass.get();

		SetSceneContext(cur_renderQueue->GetSceneContext());

		if (cur_renderPass != prePass)
		{
			cur_renderPass->Begine();
		}
		
		cur_renderQueue->Render(cur_renderPass);

		if (cur_renderPass != nextPass)
		{
			cur_renderPass->End();
		}
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
			if (tech->GetTransluce())
			{
				continue;
			}

			m_pRenderQueue->AddRenderObj(proxy->GetRenderOrder(), renderable, tech);
		}
	}

	void GbufferStep::Render()
	{
		m_pRenderQueue->Render(m_pRenderPass.get());
	}

	TransluceStep::TransluceStep()
	{

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
			if (!tech->GetTransluce())
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
}


