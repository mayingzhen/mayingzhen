#pragma once

#include "RenderStep.h"


namespace ma
{
	class RenderPass;
	//class RenderStep;
	class DeferredShadow;
	class HDRPostProcess;
	class SMAAPostProcess;
	class FrameBuffer;
	class Scene;
	class PostProcessStep;


	class DeferredShading : public Referenced
	{
	public:
		DeferredShading();

		~DeferredShading();

		void	Init();

		RenderPass* GetGpufferPass() { return m_pGbufferPass.get(); }
		RenderPass* GetTranslucePass() { return m_pTranslucePass.get(); }

	private:

		void	SetupBasePass();

		void	SetupLightPass();

		void	SetupHDRPass();

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

		RefPtr<RenderPass>		m_pTranslucePass;

		RefPtr<RenderPass>		m_pLightPass;

		RefPtr<RenderPass>		m_pBackBaufferPass;

		RefPtr<RenderPass>		m_pLinearDepthPass;

		RefPtr<PostProcessStep> m_lastStep;

		RefPtr<Technique>		m_pAmbientLight;

		bool					m_bHDREnable = true;

		bool					m_bShadowMapEnable = false;

	public:
		RefPtr<RenderStep>		m_pGbufferStep;

		RefPtr<RenderStep>		m_pTransluceStep;

		RefPtr<RenderStep>		m_pLightStep;
	};

}




