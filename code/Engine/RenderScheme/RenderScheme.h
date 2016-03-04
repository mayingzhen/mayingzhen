#ifndef  _RenderScheme__H__
#define  _RenderScheme__H__



namespace ma
{
	class ShadingPass;
	class RenderPass;
	class GBufferPass;
	class DeferredLightPass;
	class DeferredShadowPass;
	class Scene;

	class  RenderScheme : public Referenced
	{
	public:

		enum Type
		{
			Forward,
			DeferredShading,
		};

		RenderScheme(Scene* pScene);

		void	Init();

		void	ShoutDown();

		void	Render();

		void	AddRenderPass(RenderPass* pPass);

		GBufferPass* GetGBufferPass() const {return	m_pGBufferPass;}
		void AddGBufferPass();

		DeferredShadowPass*	GetDeferredShadowPass() const { return m_pDeferredShadowPass; }
		void AddDeferredShadowPass();
		
		DeferredLightPass*	GetDeferredLightPass() const { return m_pDeferredLightPass; }
		void AddDeferredLightPass();

		ShadingPass* GetShadingPass() const { return m_pShadingPass; }
		void AddShadingPass();

	private:
		typedef std::vector< RefPtr<RenderPass> >	VEC_RENDERPASS;
		VEC_RENDERPASS	m_arrRenderPass;

		GBufferPass*		m_pGBufferPass;
		DeferredShadowPass*	m_pDeferredShadowPass;
		DeferredLightPass*	m_pDeferredLightPass;
		ShadingPass*		m_pShadingPass;

		Scene*			m_pScene;
	};

	RefPtr<RenderScheme> CreateRenderScheme(RenderScheme::Type eType,Scene* pScene);

}

#endif

