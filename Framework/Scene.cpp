#include "Scene.h"


Scene::Scene()
{
	m_pRootNode = NULL;
}

Scene::~Scene()
{

}

void Scene::Tick(float fTimeElapsed)
{
	if (m_pRootNode == NULL)
		return;

	m_pRootNode->Update(fTimeElapsed);

	m_pRootNode->ParalleUpdate(fTimeElapsed);

	m_pRootNode->SyncWorld();

	m_fAccPhyTime += fTimeElapsed;
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

	m_pRootNode->LateUpdate(fTimeElapsed);

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