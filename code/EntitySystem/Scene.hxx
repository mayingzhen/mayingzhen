#include "Scene.h"

namespace ma
{
	IMPL_OBJECT(Scene,Object)

	Scene::Scene()
	{
		m_pRootNode = new SceneNode(NULL,this);
	}

	Scene::~Scene()
	{
		SAFE_DELETE(m_pRootNode);
	}

	void Scene::TravelScene(SceneVisiter* pVisiter)
	{	
		m_pRootNode->TravelScene(pVisiter);
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
}



