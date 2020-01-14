#pragma once

#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	class RenderPass;
	class DeferredShading;
	class DeferredShadow;
	class HDRPostProcess;
	class SMAAPostProcess;
	class FrameBuffer;
	class Scene;
	class PostProcessStep;

	struct RenderStep : public Referenced
	{
		RenderStep()
		{
			m_pRenderQueue[0] = new RenderQueue;
			m_pRenderQueue[1] = new RenderQueue;
		}

		virtual void Render();

		virtual void Reset() {}

		RefPtr<RenderQueue> m_pRenderQueue[2];
		RefPtr<RenderPass> m_pRenderPass;
		Rectangle m_veiwPort;
	};

	class RenderScheme : public RenderStep
	{
	public:
		RenderScheme();

		~RenderScheme();

		void	Init();

		void	Reset();

		void	Shoutdown();

		void	Render();

		DeferredShadow*	GetDeferredShadow() const { return m_pDeferredShadow.get(); }

	private:

		void SetupBasePass();

		void SetupLightPass();

		void SetupHDRPass();

		void ComputePass();

		void BasePass();

		void LightPass();

		void HDRPass();

	private:
		RefPtr<DeferredShadow>	m_pDeferredShadow;

		//Gbuffer
		RefPtr<Texture>			m_pDepthTex;
		RefPtr<SamplerState>	m_pDepthSampler;

		RefPtr<Texture>			m_pBaseColor;
		RefPtr<SamplerState>	m_pBaseSampler;

		RefPtr<Texture>			m_pNormalTex;
		RefPtr<SamplerState>	m_pNormalSampler;

		RefPtr<Texture>			m_pLinearDepthTex;

		RefPtr<Texture>			m_pHDRColorTex;

		RefPtr<Technique>		m_pTecLinearDepth;
		RefPtr<SamplerState>	m_pLinearDepthSampler;

		RefPtr<RenderPass>		m_pGbufferPass;

		RefPtr<RenderPass>		m_pLightPass;

		RefPtr<RenderPass>		m_pBackBaufferPass;

		RefPtr<RenderPass>		m_pLinearDepthPass;

		RefPtr<PostProcessStep> m_lastStep;

		bool					m_bHDREnable = false;
	};

}




