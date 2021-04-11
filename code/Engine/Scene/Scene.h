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

		void					Reset(uint32_t nWidth,uint32_t nHeight);

		CullTree*				GetCullTree() const {return m_pCullTree.get();}

		SceneNode*				GetRootNode() const {return m_pRootNode.get();}

		SceneNode*				CreateSceneNode();

		SceneNode*				CreateSceneNode(const char* pPath);

		void					SetCamera(Camera* pCamera) {m_pCamera = pCamera;}
		Camera*					GetCamera() const {return m_pCamera.get();}

		const ColourValue&		GetAmbientColor() const{return m_cAmbientColor;}
		void					SetAmbientColor(const ColourValue& cAmbientColor) {m_cAmbientColor = cAmbientColor;}
		DirectonalLight*		GetMainDirLight() {return m_pMainDirLight.get();}

	private:
		RefPtr<SceneNode>		m_pRootNode;

		RefPtr<CullTree>		m_pCullTree;

		std::string				m_sName;
		
		RefPtr<Camera>			m_pCamera;

		ColourValue				m_cAmbientColor;
		RefPtr<DirectonalLight> m_pMainDirLight;
	};

	RefPtr<Scene> CreateScene();
}

