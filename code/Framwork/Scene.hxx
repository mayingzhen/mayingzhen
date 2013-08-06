#include "Scene.h"

namespace ma
{
	IMPL_OBJECT(Scene,Object)

	Scene::Scene()
	{
		m_pPhyScene = NULL;
		if (GetPhysicsDevice())
		{
			m_pPhyScene = GetPhysicsDevice()->CreatePhysicsScene();
		}

		m_pRootNode = new SceneNode(this,"RootNode");
	}

	Scene::~Scene()
	{
		SAFE_DELETE(m_pRootNode);
		
		if (GetPhysicsDevice())
		{
			GetPhysicsDevice()->DestoryPhysicsScene(m_pPhyScene);
		}
		m_pPhyScene = NULL;
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

		if (m_pPhyScene)
		{
			m_pPhyScene->BeginSimulation(fElapsedTime);

			m_pPhyScene->EndSimulation();
		}
		
	}

	class SceneRender : public SceneVisiter
	{
		virtual bool VisiteComponent(Component* pComp)
		{
			pComp->Render();
			return true;
		}
	};

	void Scene::Render(Camera* pCmera)
	{
		m_pCurCamera = pCmera;
		Material::SetAuotBingCamera(pCmera);
		
		SceneRender sceneRender;
		TravelScene(&sceneRender);
	}

	void Scene::Awak()
	{
		//if (m_pRootNode)
		//	m_pRootNode->Awak();
	}

	class SceneStart : public SceneVisiter
	{
		virtual bool VisiteComponent(Component* pComp)
		{
			pComp->Start();
			return true;
		}
	};

	class SceneStop : public SceneVisiter
	{
		virtual bool VisiteComponent(Component* pComp)
		{
			pComp->Stop();
			return true;
		}
	};



	void Scene::Start()
	{
		if (m_pPhyScene)
			m_pPhyScene->Start();

		SceneStart sceneStart;
		TravelScene(&sceneStart);
	}	

	void Scene::Stop()
	{
		if (m_pPhyScene)
			m_pPhyScene->Stop();

		SceneStop sceneStop;
		TravelScene(&sceneStop);
	}

	void Scene::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		m_pRootNode->Serialize(sl,"RootObject");
	
		sl.EndSection();
	}

	GameObject*	Scene::CreateGameObject(const char* pName)
	{
		ASSERT(m_pRootNode);
		if (m_pRootNode == NULL)
			return NULL;

		SceneNode* pNode = m_pRootNode->AddChildNode(pName);
		GameObject* pGameObj = pNode->CreateGameObject();		
		return pGameObj;
	}
}



