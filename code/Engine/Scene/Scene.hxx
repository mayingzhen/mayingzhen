#include "Scene.h"
#include "Octree.h"
#include "ParallelCull.h"

namespace ma
{
	Scene::Scene(const char* pszName)
	{
		m_pRootNode = new SceneNode(this);
		m_sName = pszName ? pszName : "";

		m_pCamera = CreateCamera();
		m_pRootNode->AddChild(m_pCamera.get());

		m_pCullTree = new ParallelCull();

		SceneNode* pMainLightNode = CreateSceneNode();
		m_pRootNode->AddChild(pMainLightNode);
		m_pMainDirLight = CreateDirectonalLight();
		pMainLightNode->AddComponent(m_pMainDirLight.get());

		m_cAmbientColor = ColourValue::Black;
	}

	Scene::~Scene()
	{
	}

	void Scene::Reset(uint32_t nWidth,uint32_t nHeight)
	{
		float fAspect = (float)nWidth / (float)nHeight;
		float fFOV = m_pCamera->GetFov();
		float fNearClip = m_pCamera->GetNearClip();
		float fFarClip = m_pCamera->GetFarClip();
		m_pCamera->SetPerspective(fFOV,fAspect,fNearClip,fFarClip);
	}

	SceneNode* Scene::CreateSceneNode()
	{
		RefPtr<SceneNode> pSceneNode = new SceneNode(NULL,NULL);
		m_pRootNode->AddChild( pSceneNode.get() );
		return pSceneNode.get();
	}

	SceneNode* Scene::CreateSceneNode(const char* pPath)
	{
		RefPtr<SceneNode> pSceneNode = new SceneNode(NULL,NULL);
		pSceneNode->LoadFromXML(pPath);
		m_pRootNode->AddChild( pSceneNode.get() );
		return pSceneNode.get();
	}


	void Scene::Update()
	{
		ROFILE_SCOPEI("Scene::Update", 0);

		if (m_pCamera == NULL || m_pCullTree == NULL)
			return;

		m_pRootNode->Update();
	}

	RefPtr<Scene> CreateScene()
	{
		return new Scene();
	}

}
