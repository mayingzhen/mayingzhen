#ifndef  _DeferredShadowPass__H__
#define  _DeferredShadowPass__H__

namespace ma
{
	class DeferredShadowPass : public RenderPass
	{
	public:
		void Init();

		void Render();

		void ShoutDown();

		Texture* GetShadowTexture() {return m_pShadowTex;}

	private:
		Texture*		m_pShadowTex;	

		Technique*		m_pDefferedShadow;
	};

	DeferredShadowPass*	GetDeferredShadowPass();
}


#endif
