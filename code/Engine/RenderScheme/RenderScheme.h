#pragma once

namespace ma
{
	class RenderPass;
	class DeferredShading;
	class DeferredShadow;
	class HDRPostProcess;
	class SMAAPostProcess;
	class AlchemyAo;
	class FrameBuffer;
	class Scene;

	class RenderScheme : public Referenced
	{
	public:
		RenderScheme(Scene* pScene);

		void	Init();

		void	Reset();

		void	Shoutdown();

		void	Render();

		DeferredShadow*	GetDeferredShadow() const { return m_pDeferredShadow.get(); }
		DeferredShading* GetDeferredShading() const { return m_pDeferredShading.get(); }
		
		SamplerState* GetSceneDiffuse() const {return m_pDiffTempSampler.get();}
		SamplerState* GetSceneDepth() const {return m_pDepthSampler.get();}
		SamplerState* GetSceneNormal() const {return m_pNormalSampler.get();}

		void SetSMAAEnabled(bool b);
		bool GetSMAAEnabled() const;

		void SetDeferredShadingEnabled(bool b);
		bool GetDeferredShadingEnabled() const;

		void SetDeferredShadowEnabled(bool b);
		bool GetDeferredShadowEnabled() const;

		void SetSSAOEnabled(bool b);
		bool GetSSAOEnabled() const;
		
	private:
		RefPtr<DeferredShadow>	m_pDeferredShadow;
		RefPtr<DeferredShading>	m_pDeferredShading;

		RefPtr<HDRPostProcess>	m_pHDR;
		RefPtr<SMAAPostProcess> m_pSMAA;
		RefPtr<AlchemyAo>		m_pSSAO;

		RefPtr<Texture>			m_pDiffuseTex;

		RefPtr<Texture>			m_pDepthTex;
		RefPtr<Texture>			m_pLinearDepthTex;
		RefPtr<Texture>			m_pDiffTemp;
		RefPtr<Texture>			m_pNormalTex;

		RefPtr<Technique>		m_pTecLinearDepth;

		RefPtr<SamplerState>	m_pDepthSampler;
		RefPtr<SamplerState>	m_pLinearDepthSampler;
		RefPtr<SamplerState>	m_pNormalSampler;
		RefPtr<SamplerState>	m_pDiffTempSampler;

		Scene*					m_pScene;
	};

}




