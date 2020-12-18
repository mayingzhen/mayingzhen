#pragma once

#include "Engine/RenderScheme/RenderScheme.h"

namespace ma
{
	class SceneNode;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;
	class CullTree;
	class MainRenderStep;
	class RenderComponent;
	class Texture;
	//class RenderQueue;
	class DirectonalLight;

	class Scene : public Referenced
	{
	
	public:
		Scene(const char* pszName = NULL);

		~Scene();

		void					Update();

		//void					Render();

		void					Reset(uint32_t nWidth,uint32_t nHeight);

		CullTree*				GetCullTree() const {return m_pCullTree.get();}

		SceneNode*				GetRootNode() const {return m_pRootNode.get();}

		SceneNode*				CreateSceneNode();

		SceneNode*				CreateSceneNode(const char* pPath);

		void					SetCamera(Camera* pCamera) {m_pCamera = pCamera;}
		Camera*					GetCamera() const {return m_pCamera.get();}

		//uint32_t				GetVisibleNodeNum() const {return (uint32_t)m_arrRenderComp.size();}
		//RenderComponent*		GetVisibleNodeByIndex(uint32_t index) const {return m_arrRenderComp[index].get();}

// 		float					GetViewMinZ() const {return m_viwMinZ;}
// 		float					GetViewMaxZ() const {return m_viwMaxZ;}
// 		void					SetViewMinMaxZ(float fViewMinZ, float fViewMaxZ) {m_viwMinZ = fViewMinZ; m_viwMaxZ = fViewMaxZ;}

// 		void					AddParallelUpdate(Component* pComponent);
// 		void					AddParallelShow(Component* pComponent);

		const ColourValue&		GetAmbientColor() const{return m_cAmbientColor;}
		void					SetAmbientColor(const ColourValue& cAmbientColor) {m_cAmbientColor = cAmbientColor;}
		DirectonalLight*		GetMainDirLight() {return m_pMainDirLight.get();}

	private:
		RefPtr<SceneNode>		m_pRootNode;

		RefPtr<CullTree>		m_pCullTree;

		std::string				m_sName;
		
		RefPtr<Camera>			m_pCamera;

		//typedef std::vector< RefPtr<RenderComponent> > VEC_RENDERCOMP;
		//VEC_RENDERCOMP			m_arrRenderComp;

		//typedef std::vector< RefPtr<Component> > VEC_COMP;
		//VEC_COMP				m_vecParallelUpdate;
		//VEC_COMP				m_vecParallelShow;
		
		//float					m_viwMinZ;
		//float					m_viwMaxZ;

		ColourValue				m_cAmbientColor;
		RefPtr<DirectonalLight> m_pMainDirLight;
	};

	RefPtr<Scene> CreateScene();
}

