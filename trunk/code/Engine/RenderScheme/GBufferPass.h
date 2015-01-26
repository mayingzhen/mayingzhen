#ifndef  _GBufferPass__H__
#define  _GBufferPass__H__

namespace ma
{
	class GBufferPass : public RenderPass
	{
	public:
		GBufferPass(Scene* pScene);

		void		Init();

		void		Render();

		void		ShoutDown();

		Texture*	GetSceneDepth() {return m_pDepthTex.get();}

		Texture*	GetSceneNormal() {return m_pNormalTex.get();}

	private:
		RefPtr<Texture>	m_pDepthTex;

		RefPtr<Texture>	m_pNormalTex;
	};

	//GBufferPass*	GetBuffferPass();
}


#endif