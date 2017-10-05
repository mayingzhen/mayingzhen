#ifndef  _RenderScheme__H__
#define  _RenderScheme__H__



namespace ma
{
	class RenderPass;
	class DeferredShadingPass;
	class DeferredShadowPass;
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

		DeferredShadowPass*	GetDeferredShadowPass() const { return m_pDeferredShadowPass.get(); }
		DeferredShadingPass* GetDeferredShadingPass() const { return m_pDeferredShadingPass.get(); }
		
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
		RefPtr<DeferredShadowPass>	m_pDeferredShadowPass;
		RefPtr<DeferredShadingPass>	m_pDeferredShadingPass;

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

		//RefPtr<FrameBuffer>		m_pFrameBuffer;

		Scene*					m_pScene;
	};


}

#endif

