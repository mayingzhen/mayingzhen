#ifndef  _Scene__H__
#define  _Scene__H__

#include "Engine/RenderScheme/RenderScheme.h"

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
	class RenderQueue;
	class ShadowMapFrustum;


	class Scene : public Referenced
	{
	public:
		class CCallback
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

		~Scene();

		void					Render();

		void					Update();	

		void					Reset();

		CullTree*				GetCullTree() const {return m_pCullTree.get();}

		SceneNode*				GetRootNode() const {return m_pRootNode.get();}

		SceneNode*				CreateSceneNode();

		SceneNode*				CreateSceneNode(const char* pPath);

		void					SetCamera(Camera* pCamera) {m_pCamera = pCamera;}
		Camera*					GetCamera() const {return m_pCamera.get();}

		void					SetViewport(const Rectangle& viewPort) {m_viewport = viewPort;}
		const Rectangle&		GetViewport() const {return m_viewport;}	

		RenderScheme*			GetRenderScheme() const { return m_pRenderScheme.get(); }
		void					SetRenderScheme(RenderScheme::Type eType);

		UINT					GetVisibleNodeNum() const {return m_arrRenderComp.size();}
		RenderComponent*		GetVisibleNodeByIndex(UINT index) const {return m_arrRenderComp[index].get();}

		RenderQueue*			GetRenderQueue();

		void					SetCallback(CCallback* pCallback){m_pCallback = pCallback;}
		CCallback*				GetCallback() const{return m_pCallback;}

		float					GetViewMinZ() const {return m_viwMinZ;}
		float					GetViewMaxZ() const {return m_viwMaxZ;}
		void					SetViewMinMaxZ(float fViewMinZ, float fViewMaxZ) {m_viwMinZ = fViewMinZ; m_viwMaxZ = fViewMaxZ;}
		
		void					OnFlushFrame();

		void					AddParallelUpdate(Component* pComponent);
		void					AddParallelShow(Component* pComponent);

		void					AddLight(Light* pLight);
		UINT					GetVisibleLightNum() const {return m_vecLight.size();}
		Light*					GetVisibleLightByIndex(UINT index) {return m_vecLight[index].get();}

		ShadowMapFrustum*		GetShadowMapFrustum() {return m_pShadowMapFrustum;}
		void					SetShadowMapFrustum(ShadowMapFrustum* pShadowMapFrustum) {m_pShadowMapFrustum = pShadowMapFrustum;}

	private:
		RefPtr<SceneNode>		m_pRootNode;

		RefPtr<CullTree>		m_pCullTree;

		std::string				m_sName;

		RefPtr<RenderScheme>	m_pRenderScheme;
		
		RefPtr<Camera>			m_pCamera;

		Rectangle				m_viewport;

		RefPtr<Texture>			m_pRenderTarget;

		typedef vector< RefPtr<RenderComponent> > VEC_RENDERCOMP;
		VEC_RENDERCOMP			m_arrRenderComp;

		typedef vector< RefPtr<Component> > VEC_COMP;
		VEC_COMP				m_vecParallelUpdate;
		VEC_COMP				m_vecParallelShow;

		typedef vector< RefPtr<Light> > VEC_LIGHT;
		VEC_LIGHT				m_vecLight;

		ShadowMapFrustum*		m_pShadowMapFrustum;

		RenderQueue*			m_pRenderQueue[2];
		
		float					m_viwMinZ;
		float					m_viwMaxZ;
			
		CCallback*				m_pCallback;
	};

	RefPtr<Scene> CreateScene();
}



#endif // _Scene__H__
