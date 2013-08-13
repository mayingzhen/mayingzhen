#ifndef  _DeferredLight__H__
#define  _DeferredLight__H__



namespace ma
{
	class UnitSphere;
	class ScreenQuad;
	class Light;
	class ShadowMapFrustum;

	class ENGINE_API  DeferredLight
	{
	public:
		void Init();

		void DoRender();

		void GBufferPass();

		void DefferedLighting();

		void ShadingPass();

	protected:
		std::vector<Renderable*>	m_SolidEntry;
		std::vector<Light*>			m_arrLight;

		RenderTarget*				m_pDepthTex;
		RenderTarget*				m_pNormalTex;
		RenderTarget*				m_pDiffuse;
		RenderTarget*				m_pSpecular;

		Material*					m_pMaterDeferred;
	};

}

#endif