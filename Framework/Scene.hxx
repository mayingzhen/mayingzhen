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
			m_pRootNode->SyncToPhysics();

			m_pPhyScene->BeginSimulation();

			m_pPhyScene->EndSimulation();
			
			m_pRootNode->SyncFromPhysics();
		}

		m_pRootNode->LateUpdate(fElapsedTime);

		m_pRootNode->SyncWorld();
	}

	void Scene::Render(Camera* pCmera)
	{
		IRender* pRender = ma::GetRender();
		if (pRender == NULL)
			return;

		if (pCmera)
		{
			pRender->SetViewMatrix( pCmera->GetViewMatrix() );
			pRender->SetProjMatrix( pCmera->GetProjMatrix() );
		}
		
		m_pRootNode->Render();
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

}


