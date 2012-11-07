#include "Scene.h"


Scene::Scene(const SceneNode* pRootNode)
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

	m_pRootNode->Update(fElapsedTime);

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

void Scene::Render(Camera* pCmaera)
{


	m_pRootNode->Render();
// 	for (UINT i = 0; i < m_vSceneNodes.size(); ++i)
// 	{
// 		if (m_vSceneNodes[i] == NULL)
// 			continue;
// 
// 		m_vSceneNodes[i].Render();
// 	}
}

void Scene::Start()
{
	m_pRootNode->Start();
}

void Scene::Stop()
{
	m_pRootNode->Stop();
}