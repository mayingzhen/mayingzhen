#include "Scene.h"
#include "Camera.h"

namespace ma
{

	Scene::Scene()
	{
		m_pRootNode = new SceneNode(this,"RootNode");
		
		m_pPhyScene = NULL;
		if (GetPhysicsDevice())
		{
			m_pPhyScene = GetPhysicsDevice()->CreatePhysicsScene();
		}
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

	void Scene::Update(float fElapsedTime)
	{
		if (m_pRootNode == NULL)
			return;

		m_pRootNode->Update();

		m_pRootNode->ParalleUpdate(fElapsedTime);

		m_fAccPhyTime += fElapsedTime;
		if (m_fAccPhyTime > m_fFixUpdateInterval)
		{
			m_pRootNode->FixedUpdate(m_fFixUpdateInterval);
			m_fAccPhyTime -= m_fFixUpdateInterval;
		}

		if (m_pPhyScene != NULL)
		{
			m_pPhyScene->BeginSimulation(fElapsedTime);

			m_pPhyScene->EndSimulation();
		}

		m_pRootNode->LateUpdate(fElapsedTime);
	}

	void Scene::Render(Camera* pCmera)
	{
		if (GetRenderDevice() == NULL)
			return;

		GetRenderDevice()->SetCamera(pCmera);

		GetRenderDevice()->BeginRender();
		
		//GetRenderDevice()->DoRender();
		//m_pRootNode->Render();

		GetRenderDevice()->EndRender();
	}

	void Scene::Start()
	{
		if (m_pPhyScene)
			m_pPhyScene->Start();

		if (m_pRootNode)
			m_pRootNode->Start();
	}

	void Scene::Stop()
	{
		if (m_pPhyScene)
			m_pPhyScene->Stop();

		if (m_pRootNode)
			m_pRootNode->Stop();
	}

	void Scene::Serialize(SerializeListener& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		if (m_pRootNode == NULL)
		{
			m_pRootNode = new SceneNode(this,"RootNode");
		}
		m_pRootNode->Serialize(sl);
	
		sl.EndSection();
	}
}



