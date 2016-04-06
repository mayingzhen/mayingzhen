#ifndef  _DeferredShadowPass__H__
#define  _DeferredShadowPass__H__

namespace ma
{
	class DeferredShadowPass : public RenderPass
	{
	public:
		DeferredShadowPass(Scene* pScene);

		void Init();
	
		void Reset();

		void Render();

		void Shoutdown();

		Texture* GetShadowTexture() {return m_pShadowTex.get();}

	private:
		RefPtr<Texture>		m_pShadowTex;	

		RefPtr<Technique>	m_pFrustumMaterial;
		RefPtr<Technique>	m_pDefferedShadow;
	};

}


#endif
