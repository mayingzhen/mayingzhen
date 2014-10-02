#ifndef  _RenderView__H__
#define  _RenderView__H__


namespace ma
{
	class RenderComponent;

	class RENDER_API RenderView : public Referenced
	{
	public:
		RenderView(Scene* pScene,Camera* pCamea,RenderScheme* pRS,Texture* pRT,Rectangle rc);

		void	Update();
		void	Render();

		Scene*	GetScene() {return m_pScene.get();}
		Camera*	GetCamera() {return m_pCamera.get();}

		UINT	GetRenderCompCount() {return m_arrRenderComp.size();}
		RenderComponent*	GetRenderCompByIndex(UINT index) {return m_arrRenderComp[index];}

	private:

		RefPtr<RenderScheme>	m_pRenderScheme;

		RefPtr<Camera>			m_pCamera;

		RefPtr<Scene>			m_pScene;

		Rectangle				m_viewport;

		RefPtr<Texture>			m_pRenderTarget;


		typedef std::vector<RenderComponent*> VEC_RENDERCOMP;
		VEC_RENDERCOMP			m_arrRenderComp;
	};
}

#endif