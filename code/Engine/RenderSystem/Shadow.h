#ifndef  _Shadow__H__
#define  _Shadow__H__

namespace ma
{
	class ShadowMapFrustum;

	class Shadow
	{
	public:
		void		Init();

		void		ShadowDepthPass();

		void		DeferredShadow();

		void		ShadowBlur();

		Texture*	GetShadowTexture() {return m_pShadowTex/*->GetTexture()*/;}

		float		GetShadowMapSize() {return m_fShadowMapSize;}
		
		bool		IsHWShadowMap() {return m_bHWShadowMap;}

	protected:

		Texture*					m_pShadowTex;	

		std::vector<ShadowMapFrustum*>	m_arrSMF;

		//Material*						 m_pMaterial;
		//Effect*							 m_pEffect;
		Technique*						m_pDefferedShadow;

		float						     m_fShadowMapSize;

		bool							 m_bHWShadowMap;
	};

	Shadow*		GetShadowSystem();

}



#endif
