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

	void Scene::Update(float fElapsedTime)
	{
		//GetScriptDevice()->Update();

		//GetAnimationDevice()->Update();

// 		if (m_pRootNode == NULL)
// 			return;
// 
// 		m_pRootNode->Update();
// 
// 		m_pRootNode->ParalleUpdate(fElapsedTime);
// 
// 		m_fAccPhyTime += fElapsedTime;
// 		if (m_fAccPhyTime > m_fFixUpdateInterval)
// 		{
// 			m_pRootNode->FixedUpdate(m_fFixUpdateInterval);
// 			m_fAccPhyTime -= m_fFixUpdateInterval;
// 		}
// 
// 		if (m_pPhyScene != NULL)
// 		{
// 			m_pPhyScene->BeginSimulation(fElapsedTime);
// 
// 			m_pPhyScene->EndSimulation();
// 		}
// 
// 		m_pRootNode->LateUpdate(fElapsedTime);

		//if (m_pRootGameObject)
		//	m_pRootGameObject->Update();

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
		if (GetRenderDevice() == NULL)
			return;

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

	void Scene::Start()
	{
		if (m_pPhyScene)
			m_pPhyScene->Start();

		//if (m_pRootNode)
		//	m_pRootNode->Start();
	}

	void Scene::Stop()
	{
		if (m_pPhyScene)
			m_pPhyScene->Stop();

		//if (m_pRootNode)
		//	m_pRootNode->Stop();
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



