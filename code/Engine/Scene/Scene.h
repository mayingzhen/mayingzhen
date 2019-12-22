#pragma once

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
	//class RenderQueue;
	class DirectonalLight;


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

		void					Update();

		void					Render();

		void					Reset(uint32_t nWidth,uint32_t nHeight);

		CullTree*				GetCullTree() const {return m_pCullTree.get();}

		SceneNode*				GetRootNode() const {return m_pRootNode.get();}

		SceneNode*				CreateSceneNode();

		SceneNode*				CreateSceneNode(const char* pPath);

		void					SetCamera(Camera* pCamera) {m_pCamera = pCamera;}
		Camera*					GetCamera() const {return m_pCamera.get();}

		uint32_t				GetVisibleNodeNum() const {return (uint32_t)m_arrRenderComp.size();}
		RenderComponent*		GetVisibleNodeByIndex(uint32_t index) const {return m_arrRenderComp[index].get();}

		void					SetCallback(CCallback* pCallback){m_pCallback = pCallback;}
		CCallback*				GetCallback() const{return m_pCallback;}

		float					GetViewMinZ() const {return m_viwMinZ;}
		float					GetViewMaxZ() const {return m_viwMaxZ;}
		void					SetViewMinMaxZ(float fViewMinZ, float fViewMaxZ) {m_viwMinZ = fViewMinZ; m_viwMaxZ = fViewMaxZ;}

		void					AddParallelUpdate(Component* pComponent);
		void					AddParallelShow(Component* pComponent);

		uint32_t				GetVisibleLightNum() const {return (uint32_t)m_vecRenderLight.size();}
		Light*					GetVisibleLightByIndex(uint32_t index) {return m_vecRenderLight[index].get();}

		const Vector3&			GetAmbientColor() const{return m_cAmbientColor;}
		void					SetAmbientColor(const Vector3& cAmbientColor) {m_cAmbientColor = cAmbientColor;}
		DirectonalLight*		GetMainDirLight() {return m_pMainDirLight.get();}

		void					AddRenderLight(Light* pLight);

	private:
		RefPtr<SceneNode>		m_pRootNode;

		RefPtr<CullTree>		m_pCullTree;

		std::string				m_sName;
		
		RefPtr<Camera>			m_pCamera;

		typedef std::vector< RefPtr<RenderComponent> > VEC_RENDERCOMP;
		VEC_RENDERCOMP			m_arrRenderComp;

		typedef std::vector< RefPtr<Component> > VEC_COMP;
		VEC_COMP				m_vecParallelUpdate;
		VEC_COMP				m_vecParallelShow;

		typedef std::vector< RefPtr<Light> > VEC_LIGHT;
		VEC_LIGHT				m_vecRenderLight;
		
		float					m_viwMinZ;
		float					m_viwMaxZ;
			
		CCallback*				m_pCallback;

		Vector3					m_cAmbientColor;
		RefPtr<DirectonalLight> m_pMainDirLight;
	};

	RefPtr<Scene> CreateScene();
}

