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

	void Scene::Reset()
	{
		m_pRootNode = NULL;
		m_pCullTree = NULL;

		m_pCullTree = new Octree();

		m_pRootNode = new SceneNode(this,"RootGameObject");		
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



