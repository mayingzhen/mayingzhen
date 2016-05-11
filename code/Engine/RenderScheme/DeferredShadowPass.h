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
		void CreateSimpleLightFrustumMesh();

	private:
		RefPtr<Texture>		m_pShadowTex;	

		RefPtr<Technique>	m_pFrustumVolume;
		RefPtr<Technique>	m_pDefferedShadow;

		RefPtr<Renderable>	m_pRenderable;

		RefPtr<DirectonalLight> m_ShadowLight;
	};

}


#endif
