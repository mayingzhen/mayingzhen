#ifndef  _DeferredShadowPass__H__
#define  _DeferredShadowPass__H__

namespace ma
{
	class DeferredShadowPass : public RenderPass
	{
		DECL_OBJECT(DeferredShadowPass)

	public:
		void Init();

		void Render();

		void ShoutDown();

		Texture* GetShadowTexture() {return m_pShadowTex.get();}

	private:
		RefPtr<Texture>		m_pShadowTex;	

		RefPtr<Technique>	m_pDefferedShadow;
	};

	DeferredShadowPass*	GetDeferredShadowPass();
}


#endif