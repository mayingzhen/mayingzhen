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
		DeferredShading* GetDeferredShading() const { return m_pDeferredShading.get(); }
		
		SamplerState* GetSceneDiffuse() const {return m_pDiffTempSampler.get();}
		SamplerState* GetSceneDepth() const {return m_pDepthSampler.get();}
		SamplerState* GetSceneNormal() const {return m_pNormalSampler.get();}

		void SetDeferredShadingEnabled(bool b);
		bool GetDeferredShadingEnabled() const;
		
	private:
		RefPtr<DeferredShadow>	m_pDeferredShadow;
		RefPtr<DeferredShading>	m_pDeferredShading;

		RefPtr<Texture>			m_pDiffuseTex;
		RefPtr<RenderPass>		m_pHDRFB;

		RefPtr<Texture>			m_pDepthTex;
		RefPtr<Texture>			m_pLinearDepthTex;
		RefPtr<Texture>			m_pDiffTemp;
		RefPtr<Texture>			m_pNormalTex;

		RefPtr<Technique>		m_pTecLinearDepth;

		RefPtr<SamplerState>	m_pDepthSampler;
		RefPtr<SamplerState>	m_pLinearDepthSampler;
		RefPtr<SamplerState>	m_pNormalSampler;
		RefPtr<SamplerState>	m_pDiffTempSampler;

		RefPtr<RenderPass>		m_pShadingPass;

		RefPtr<RenderPass>		m_pBackBaufferPass;

		RefPtr<RenderPass>		m_pTemPass;

		RefPtr<RenderPass>		m_pLinearDepthPass;

		RefPtr<PostProcessStep> m_lastStep;
	};

}




