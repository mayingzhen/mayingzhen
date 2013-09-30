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
		DeferredLight();

		void		Init();

		void		GBufferPass();

		void		DefferedLighting();

		Texture*	GetSceneDepth() {return m_pDepthTex->GetTexture();}

		Texture*	GetSceneNormal() {return m_pNormalTex->GetTexture();}

		Texture*	GetTextureLightDiffuse() {return m_pDiffuse->GetTexture();}

		Texture*	GetTextureightSpecular() {return m_pSpecular->GetTexture();}


	protected:

		RenderTarget*				m_pDepthTex;
		RenderTarget*				m_pNormalTex;
		RenderTarget*				m_pDiffuse;
		RenderTarget*				m_pSpecular;

		Material*					m_pMaterDeferred;
	};

	ENGINE_API	DeferredLight*	GetDeferredLight();
	ENGINE_API	void			SetDeferredLight(DeferredLight* pDeffLight);
}

#endif

