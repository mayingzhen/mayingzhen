#include "Scene.h"


namespace ma
{
	IMPL_OBJECT(Scene,Object)

	static Scene* gpScene = NULL; 

	Scene* GetSceneSystem()
	{
		return gpScene;
	}

	void SetSceneSystem(Scene* pScene)
	{
		gpScene = pScene;
	}

	Scene::Scene()
	{
		m_pRootNode = new SceneNode(NULL);
	}

	Scene::~Scene()
	{
		SAFE_DELETE(m_pRootNode);
	}

	void Scene::TravelScene(SceneVisiter* pVisiter)
	{	
		m_pRootNode->TravelScene(pVisiter);
	}

	void Scene::Update()
	{
	}

	void Scene::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		m_pRootNode->Serialize(sl,"RootObject");
	
		sl.EndSection();
	}

	void Scene::AddSceneNode(SceneNode* pSceneNode)
	{
		m_pRootNode->AddChildNode(pSceneNode);
	}

	SceneNode* Scene::CreateSceneNode(GameObject* pGameObj)
	{
		SceneNode* pSceneNode = new SceneNode(pGameObj);
		m_pRootNode->AddChildNode(pSceneNode);
		return pSceneNode;
	}
}



