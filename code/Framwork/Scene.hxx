#include "Scene.h"

namespace ma
{
	IMPL_OBJECT(Scene,Object)

	Scene::Scene()
	{
		m_pRootNode = new SceneNode(NULL,this);
	}

	Scene::~Scene()
	{
		SAFE_DELETE(m_pRootNode);
	}

	void Scene::TravelScene(SceneVisiter* pVisiter)
	{	
		m_pRootNode->TravelScene(pVisiter);
	}

	class SceneUpdate : public SceneVisiter
	{
		virtual bool VisiteComponent(Component* pComp)
		{
			pComp->Update();
			return true;
		}
	};

	void Scene::Update(float fElapsedTime)
	{
		SceneUpdate sceneUpdate;
		TravelScene(&sceneUpdate);
	}

// 	class SceneRender : public SceneVisiter
// 	{
// 		virtual bool VisiteComponent(Component* pComp)
// 		{
// 			pComp->Render();
// 			return true;
// 		}
// 	};
// 
// 	void Scene::Render(Camera* pCmera)
// 	{
// 		m_pCurCamera = pCmera;
// 		GetRenderSystem()->SetCamera(pCmera);
// 		
// 		SceneRender sceneRender;
// 		TravelScene(&sceneRender);
// 	}

// 	void Scene::Awak()
// 	{
// 		//if (m_pRootNode)
// 		//	m_pRootNode->Awak();
// 	}

	class SceneStart : public SceneVisiter
	{
		virtual bool VisiteComponent(Component* pComp)
		{
			if (pComp)
				pComp->Start();
			
			return true;
		}
	};

	class SceneStop : public SceneVisiter
	{
		virtual bool VisiteComponent(Component* pComp)
		{
			if (pComp)
				pComp->Stop();
			
			return true;
		}
	};

	void Scene::Start()
	{
		SceneStart sceneStart;
		TravelScene(&sceneStart);
	}	

	void Scene::Stop()
	{
		SceneStop sceneStop;
		TravelScene(&sceneStop);
	}

	void Scene::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		m_pRootNode->Serialize(sl,"RootObject");
	
		sl.EndSection();
	}

	void Scene::AddSceneNode(SceneNode* pSceneNode)
	{
		m_pRootNode->AddChildNode(pSceneNode);
	}

// 	GameObject*	Scene::CreateGameObject(const char* pName)
// 	{
// 		ASSERT(m_pRootNode);
// 		if (m_pRootNode == NULL)
// 			return NULL;
// 
// 		SceneNode* pNode = m_pRootNode->CreateChildNode(pName);
// 		GameObject* pGameObj = pNode->CreateGameObject();		
// 		return pGameObj;
// 	}
// 
// 	void Scene::DeleteGameObject(GameObject* pGameObj)
// 	{
// 		ASSERT(pGameObj);
// 		if (pGameObj == NULL)
// 			return;
// 
// 		SceneNode* pSceneNode = pGameObj->GetSceneNode();
// 		ASSERT(pSceneNode);
// 		if (pSceneNode == NULL)
// 		{
// 			SAFE_DELETE(pGameObj);
// 			return ;
// 		}
// 
// 		SceneNode* pParent = pSceneNode->GetParent();
// 		ASSERT(pParent);
// 		if (pParent == NULL)
// 		{
// 			SAFE_DELETE(pSceneNode);
// 			SAFE_DELETE(pGameObj);
// 			return;
// 		}
// 		
// 		pSceneNode->DeleteGameObject(pGameObj);
// 		pParent->DeleteChildNode(pSceneNode);
// 
// 	}
}



