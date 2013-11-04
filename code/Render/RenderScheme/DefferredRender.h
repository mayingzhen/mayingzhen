#ifndef  _DefferredRender__H__
#define  _DefferredRender__H__



namespace ma
{

	class ShadowDepthPass;
	class GBufferPass;
	class DeferredShadowPass;
	class DeferredLightPass;
	class ShadingPass;

	class RENDER_API  DefferredRender : public RenderScheme
	{
	public:

		DefferredRender();


	private:
		ShadowDepthPass* m_pShadowDepthPas;

		GBufferPass* m_pGBufferPass;

		DeferredShadowPass* m_pShadowPass;

		DeferredLightPass* m_pLightPass;
	
		ShadingPass* m_pShadingPass;

	};
}

#endif


