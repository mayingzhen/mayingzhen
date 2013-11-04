#ifndef  _GBufferPass__H__
#define  _GBufferPass__H__

namespace ma
{
	class GBufferPass : public RenderPass
	{
	public:
		void		Init();

		void		Render();

		void		ShoutDown();


		Texture*	GetSceneDepth() {return m_pDepthTex;}

		Texture*	GetSceneNormal() {return m_pNormalTex;}

	private:
		Texture*	m_pDepthTex;

		Texture*	m_pNormalTex;
	};

	GBufferPass*	GetBuffferPass();
}


#endif
