#include "Scene.h"


Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Tick()
{
	for (UINT i = 0; i < m_vSceneNodes.size(); ++i)
	{
		if (m_vSceneNodes[i] == NULL)
			continue;

		//m_vSceneNodes[i].Tick();
	}
}

void Scene::Render()
{
	for (UINT i = 0; i < m_vSceneNodes.size(); ++i)
	{
		if (m_vSceneNodes[i] == NULL)
			continue;

		m_vSceneNodes[i].Render();
	}
}