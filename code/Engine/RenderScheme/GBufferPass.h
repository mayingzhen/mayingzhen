#ifndef  _GBufferPass__H__
#define  _GBufferPass__H__

namespace ma
{
	class GBufferPass : public RenderPass
	{
	public:
		GBufferPass(Scene* pScene);

		void		Init();
		
		void		Reset();

		void		Render();

		void		Shoutdown();
		
		Texture*	GetSceneDiffuse() {return m_pDiffuse.get();}

		Texture*	GetSceneDepth() {return m_pDepthTex.get();}

		Texture*	GetSceneNormal() {return m_pNormalTex.get();}

	private:
		void		RenderObjetList();

		void		CopyDiffuse();

		void		BlendDiffuse();

	private:
		RefPtr<Texture>	m_pDepthTex;

		RefPtr<Texture>	m_pNormalTex;

		RefPtr<Texture> m_pDiffuse;

		RefPtr<Texture> m_pDiffuseCopy;
	};
}


#endif
