#ifndef  _RenderScheme__H__
#define  _RenderScheme__H__



namespace ma
{
	class RenderPass;
	class DeferredShadingPass;
	class DeferredShadowPass;
	class HDRPostProcess;
	class SMAAPostProcess;
	class FrameBuffer;
	class Scene;

	class  RenderScheme : public Referenced
	{
	public:
		RenderScheme(Scene* pScene);

		void	Init();

		void	Reset();

		void	Shoutdown();

		void	Render();

		void	AddRenderPass(RenderPass* pPass);

		DeferredShadowPass*	GetDeferredShadowPass() const { return m_pDeferredShadowPass.get(); }
		
		DeferredShadingPass*	GetDeferredShadingPass() const { return m_pDeferredShadingPass.get(); }

		SamplerState* GetSceneDiffuse() const {return m_pDiffuseSampler.get();}
		SamplerState* GetSceneDepth() const {return m_pDepthSampler.get();}
		SamplerState* GetSceneNormal() const {return m_pNormalSampler.get();}

		void SetSMAAEnabled(bool b);
		bool GetSMAAEnabled() const;

		void SetDeferredShadingEnabled(bool b);
		bool GetDeferredShadingEnabled() const;

	private:
		RefPtr<DeferredShadowPass>	m_pDeferredShadowPass;
		RefPtr<DeferredShadingPass>	m_pDeferredShadingPass;

		RefPtr<HDRPostProcess>	m_pHDR;
		RefPtr<SMAAPostProcess> m_pSMAA;

		RefPtr<Texture>			m_pDepthTex;
		RefPtr<Texture>			m_pNormalTex;
		RefPtr<Texture>			m_pDiffuseTex;

		RefPtr<SamplerState>	m_pDepthSampler;
		RefPtr<SamplerState>	m_pNormalSampler;
		RefPtr<SamplerState>	m_pDiffuseSampler;

		RefPtr<FrameBuffer>		m_pFrameBuffer;

		Scene*					m_pScene;

		//Type					m_eType;
	};

	//RefPtr<RenderScheme> CreateRenderScheme(Scene* pScene);

}

#endif

