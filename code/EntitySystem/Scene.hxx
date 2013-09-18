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

	class SceneUpdate : public SceneVisiter
	{
		virtual bool VisiteComponent(Component* pComp)
		{
			pComp->Update();
			return true;
		}
	};

	void Scene::Update(float fElapsedTime)
	{
		SceneUpdate sceneUpdate;
		TravelScene(&sceneUpdate);
	}


	class SceneStart : public SceneVisiter
	{
		virtual bool VisiteComponent(Component* pComp)
		{
			if (pComp)
				pComp->Start();
			
			return true;
		}
	};

	class SceneStop : public SceneVisiter
	{
		virtual bool VisiteComponent(Component* pComp)
		{
			if (pComp)
				pComp->Stop();
			
			return true;
		}
	};

	void Scene::Start()
	{
		SceneStart sceneStart;
		TravelScene(&sceneStart);
	}	

	void Scene::Stop()
	{
		SceneStop sceneStop;
		TravelScene(&sceneStop);
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



