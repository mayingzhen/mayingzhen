#include "Scene.h"
#include "Octree.h"

namespace ma
{
	Scene::Scene(const char* pszName)
	{
		m_sName = pszName ? pszName : "";

		Reset();
	}

	SceneNode*	Scene::CreateNode(const char* pName)
	{
		SceneNode* pSceneNode = new SceneNode(this,pName);	
		m_pRootNode->AddChild(pSceneNode);
		return pSceneNode;
	}

	SceneNode*	Scene::FindNode(const char* pszName)
	{
		ASSERT(pszName);
		if (pszName == NULL)
			return NULL;

		if ( strcmp(pszName, m_pRootNode->GetName() ) == 0)
			return m_pRootNode.get();
	
		return m_pRootNode->FindChildNode(pszName);
	}

	void Scene::Reset()
	{
		m_pRootNode = NULL;
		m_pCullTree = NULL;

		m_pCullTree = new Octree();

		m_pRootNode = new SceneNode(this,"RootGameObject");		
	}

	void Scene::Update()
	{
		ASSERT(m_pRootNode);
		if (m_pRootNode == NULL)
			return;

		m_pRootNode->Update();
	}

	void Scene::Serialize(Serializer& sl, const char* pszLable)
	{
		if ( sl.IsReading() )
		{
			Reset();
		}

		sl.BeginSection(pszLable);

		sl.Serialize(*m_pRootNode,"RootObject");

		sl.EndSection();
	}

}



