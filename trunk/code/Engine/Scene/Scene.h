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
		class  CCallback
		{
		public:
			CCallback(){}
			virtual ~CCallback(){}

			virtual void OnPreUpdate(Scene* pScene) = 0;
			virtual void OnPostUpdate(Scene* pScene) = 0;
			virtual void OnPreRender(Scene* pScene) = 0;
			virtual void OnPosRender(Scene* pScene) = 0;
		};
	
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

		void					SetCallback(CCallback* pCallback){m_pCallback = pCallback;}
		CCallback*				GetCallback() const{return m_pCallback;}

		float					GetViewMinZ() const {return m_viwMinZ;}
		float					GetViewMaxZ() const {return m_viwMaxZ;}

		void					GetDirectionalLight(OUT ColourValue& color, OUT Vector3& vDir) const;
		void					SetDirectionalLight(const ColourValue& color, const Vector3& vDir);
	
	private:
		void					UpdateViewMinMaxZ();

	private:
		RefPtr<CullTree>		m_pCullTree;

		std::string				m_sName;

		RefPtr<RenderScheme>	m_pRenderScheme;

		RefPtr<Camera>			m_pCamera;

		Rectangle				m_viewport;

		RefPtr<Texture>			m_pRenderTarget;

		typedef std::vector<RenderComponent*> VEC_RENDERCOMP;
		VEC_RENDERCOMP			m_arrRenderComp;
		
		float					m_viwMinZ;
		float					m_viwMaxZ;

		ColourValue				m_cDirLight;
		Vector3					m_vDirLight;
			
		CCallback*				m_pCallback;
	};

	DeclareRefPtr(Scene);
}



#endif // _Scene__H__
