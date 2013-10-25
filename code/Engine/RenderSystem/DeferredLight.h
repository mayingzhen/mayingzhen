#ifndef  _DeferredLight__H__
#define  _DeferredLight__H__



namespace ma
{
	class UnitSphere;
	class ScreenQuad;
	class Light;
	class ShadowMapFrustum;
	class Effect;

	class ENGINE_API  DeferredLight
	{
	public:
		DeferredLight();

		void		Init();

		void		GBufferPass();

		void		DefferedLighting();

		Texture*	GetSceneDepth() {return m_pDepthTex/*->GetTexture()*/;}

		Texture*	GetSceneNormal() {return m_pNormalTex/*->GetTexture()*/;}

		Texture*	GetTextureLightDiffuse() {return m_pDiffuse/*->GetTexture()*/;}

		Texture*	GetTextureightSpecular() {return m_pSpecular/*->GetTexture()*/;}


	protected:

		Texture*				m_pDepthTex;
		Texture*				m_pNormalTex;
		Texture*				m_pDiffuse;
		Texture*				m_pSpecular;

		//Material*				m_pMaterDeferred;
		//Effect*					m_pDeferredEffect;

		Technique*				m_pAmbientLight;
		Technique*				m_pDirLight;
		Technique*				m_pPointLight;
	};

	ENGINE_API	DeferredLight*	GetDeferredLight();
	ENGINE_API	void			SetDeferredLight(DeferredLight* pDeffLight);
}

#endif

