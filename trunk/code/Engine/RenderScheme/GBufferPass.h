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
		
		Texture*	GetSceneDiffuse() {return m_pDiffuse.get();}

		Texture*	GetSceneDepth() {return m_pDepthTex.get();}

		Texture*	GetSceneNormal() {return m_pNormalTex.get();}

	private:
		RefPtr<Texture>	m_pDepthTex;

		RefPtr<Texture>	m_pNormalTex;

		RefPtr<Texture> m_pDiffuse;
	};
}


#endif
