#include "Framework/Scene.h"
#include "Framework/Camera.h"

namespace ma
{

	Scene::Scene(SceneNode* pRootNode)
	{
		m_pRootNode = pRootNode;
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
			m_pPhyScene->BeginSimulation();

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

		if (pCmera)
		{
			pRender->SetViewMatrix( pCmera->GetViewMatrix() );
			pRender->SetProjMatrix( pCmera->GetProjMatrix() );
		}
		
		m_pRootNode->Render();
	}

	void Scene::Start()
	{
		m_pRootNode->Start();
	}

	void Scene::Stop()
	{
		m_pRootNode->Stop();
	}

}


