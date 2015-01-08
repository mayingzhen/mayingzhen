#ifndef  _Scene__H__
#define  _Scene__H__


namespace ma
{
	class SceneNode;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;
	class CullTree;
	class RenderScheme;
	class RenderComponent;
	class Texture;


	class ENGINE_API Scene  : public SceneNode
	{
	
	public:
		Scene(const char* pszName = NULL);

		void					Init();

		void					ShutDown();

		void					Render();

		void					Update();	

		void					Reset();

		CullTree*				GetCullTree() const {return m_pCullTree.get();}
		SceneNode*				CreateNode(const char* pName);

		void					SetCamera(Camera* pCamera) {m_pCamera = pCamera;}
		Camera*					GetCamera() const {return m_pCamera.get();}

		void					SetViewport(const Rectangle& viewPort) {m_viewport = viewPort;}
		const Rectangle&		GetViewport() const {return m_viewport;}

		RenderScheme*			GetRenderScheme() const {return m_pRenderScheme.get();}
		void					SetRenderScheme(RenderScheme* pScheme) {m_pRenderScheme = pScheme;}			

		UINT					GetVisibleNodeNum() const {return m_arrRenderComp.size();}
		RenderComponent*		GetVisibleNodeByIndex(UINT index) const {return m_arrRenderComp[index];}

	private:
		RefPtr<CullTree>		m_pCullTree;

		std::string				m_sName;

		RefPtr<RenderScheme>	m_pRenderScheme;

		RefPtr<Camera>			m_pCamera;

		Rectangle				m_viewport;

		RefPtr<Texture>			m_pRenderTarget;

		typedef std::vector<RenderComponent*> VEC_RENDERCOMP;
		VEC_RENDERCOMP			m_arrRenderComp;
	};

	DeclareRefPtr(Scene);
}



#endif // _Scene__H__