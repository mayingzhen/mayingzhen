#ifndef  _RenderScheme__H__
#define  _RenderScheme__H__



namespace ma
{
	class RenderPass;
	class DeferredLightPass;
	class DeferredShadowPass;
	class HDRPostProcess;
	class SMAAPostProcess;
	class FrameBuffer;
	class Scene;

	class  RenderScheme : public Referenced
	{
	public:

		enum Type
		{
			Forward,
			DeferredShading,
		};

		RenderScheme(RenderScheme::Type eType,Scene* pScene);

		void	Init();

		void	Reset();

		void	Shoutdown();

		void	Render();

		void	AddRenderPass(RenderPass* pPass);

		DeferredShadowPass*	GetDeferredShadowPass() const { return m_pDeferredShadowPass.get(); }
		
		DeferredLightPass*	GetDeferredLightPass() const { return m_pDeferredLightPass.get(); }

		Texture* GetSceneDiffuse() const {return m_pDiffuseTex.get();}
		Texture* GetSceneDepth() const {return m_pDepthTex.get();}
		Texture* GetSceneNormal() const {return m_pNormalTex.get();}

		void SetSMAAEnabled(bool b);
		bool GetSMAAEnabled() const;

	private:
		RefPtr<DeferredShadowPass>	m_pDeferredShadowPass;
		RefPtr<DeferredLightPass>	m_pDeferredLightPass;

		RefPtr<HDRPostProcess>	m_pHDR;
		RefPtr<SMAAPostProcess> m_pSMAA;

		RefPtr<Texture>			m_pDepthTex;
		RefPtr<Texture>			m_pNormalTex;
		RefPtr<Texture>			m_pDiffuseTex;

		RefPtr<FrameBuffer>		m_pFrameBuffer;

		Scene*					m_pScene;

		Type					m_eType;
	};

	RefPtr<RenderScheme> CreateRenderScheme(RenderScheme::Type eType,Scene* pScene);

}

#endif

