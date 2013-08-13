#ifndef  _Shadow__H__
#define  _Shadow__H__

namespace ma
{
	class RenderTarget;
	class ShadowMapFrustum;

	class Shadow
	{
	public:
		void Init();

		void DoRender();

		void Update();

		void ShadowDepthPass();

		void DeferredShadow();

		void ShadowBlur();

	protected:

		RenderTarget*					m_pShadowTex;	

		std::vector<ShadowMapFrustum*>	m_arrSMF;
	};
}



#endif
