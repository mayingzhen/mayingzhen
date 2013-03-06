#include "Framework/Scene.h"
#include "Framework/Camera.h"

namespace ma
{

	Scene::Scene(SceneNode* pRootNode)
	{
		m_pRootNode = pRootNode;
		m_pPhyScene = NULL;

		IPhysicsDevice* pPhysicsDevice = GetPhysicsDevice();
		if (pPhysicsDevice)
		{
			m_pPhyScene = pPhysicsDevice->CreatePhysicsScene();
		}

		m_pRenderQueue = new RenderQueue();
		SetRenderQueue(m_pRenderQueue);
	}

	Scene::~Scene()
	{

	}

	void Scene::Update(float fElapsedTime)
	{
		if (m_pRootNode == NULL)
			return;

		m_pRootNode->Update();

		m_pRootNode->ParalleUpdate(fElapsedTime);

		m_pRootNode->SyncWorld();

		m_fAccPhyTime += fElapsedTime;
		if (m_fAccPhyTime > m_fFixUpdateInterval)
		{
			m_pRootNode->FixedUpdate(m_fFixUpdateInterval);
			m_fAccPhyTime -= m_fFixUpdateInterval;
		}

		m_pRootNode->SyncWorld();

		if (m_pPhyScene != NULL)
		{
			m_pPhyScene->BeginSimulation(fElapsedTime);

			m_pPhyScene->EndSimulation();
		}

		m_pRootNode->LateUpdate(fElapsedTime);

		m_pRootNode->SyncWorld();
	}

	void Scene::Render(Camera* pCmera)
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		if (m_pRenderQueue == NULL)
			return;

		m_pRenderQueue->Clear();

		if (pCmera)
		{
			pRender->SetViewMatrix( pCmera->GetViewMatrix() );
			pRender->SetProjMatrix( pCmera->GetProjMatrix() );
		}
		
		m_pRootNode->Render();

		//m_pRenderQueue->Flush();
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



