#include "EntitySystem.h"

namespace ma
{
	static EntitySystem* gpEntitySystem = NULL;

	EntitySystem* GetEntitySystem()
	{
		return gpEntitySystem;
	}

	void SetEntitySystem(EntitySystem* pEntitySystem)
	{
		gpEntitySystem = pEntitySystem;
	}

	void EntitySystem::Init()
	{
		m_pScene = new Scene();	
	}

	void EntitySystem::ShoutDown()
	{
		SAFE_DELETE(m_pScene);
	}

	void EntitySystem::Update()
	{
		m_pScene->Update(GetTimer()->GetFrameDeltaTime());
// 		for (UINT i = 0; i < m_arrMeshComp.size(); ++i)
// 		{
// 			m_arrMeshComp[i]->Update();
// 		}
	}

// 	void EntitySystem::Render()
// 	{
// 		m_pScene->Render();
// 	}

	void EntitySystem::Start()
	{
		m_pScene->Start();
// 		if (GetPhysicsSystem())
// 			GetPhysicsSystem()->Start();
// 
// 		if (GetScriptSystem())
// 			GetScriptSystem()->Start();

// 		for (UINT i = 0; i < m_arrGameObject.size(); ++i)
// 		{
// 			int nMeshComp = m_arrGameObject[i]->GetTypeComponentNumber<MeshComponent>();
// 			for (UINT j = 0; j < nMeshComp; ++j)
// 			{
// 				m_arrMeshComp.push_back(m_arrGameObject[i]->GetTypeComponentByIndex<MeshComponent>(j));
// 			}
// 		}
	}

	void EntitySystem::Stop()
	{
		m_pScene->Stop();
// 		if (GetPhysicsSystem())
// 			GetPhysicsSystem()->Stop();
// 
// 		if (GetScriptSystem())
// 			GetScriptSystem()->Stop();

		//m_arrMeshComp.clear();
	}



	void EntitySystem::Serialize(Serializer& sl)
	{
		m_pScene->Serialize(sl);
	}

	GameObject*	EntitySystem::CreateGameObject(const char* pName)
	{
		ASSERT(m_pScene);
		if (m_pScene == NULL)
			return NULL;

		GameObject* pGameObj = new GameObject(pName);//m_pScene->CreateGameObject(pName);	
		m_arrGameObject.push_back(pGameObj);

		m_pScene->AddSceneNode(pGameObj->GetSceneNode());

		return pGameObj;
	}

	void EntitySystem::DeleteGameObject(GameObject* pObject)
	{
		ASSERT(m_pScene);
		if (m_pScene == NULL)
			return;	

		std::vector<GameObject*>::iterator it;
		it = std::find(m_arrGameObject.begin(),m_arrGameObject.end(),pObject);
		ASSERT(it != m_arrGameObject.end());
		if (it == m_arrGameObject.end())
			return;

		m_arrGameObject.erase(it);

		SAFE_DELETE(pObject);
	}

	void EntitySystem::DeleteAll()
	{
		for (UINT i = 0; i < m_arrGameObject.size(); ++i)
		{
			SAFE_DELETE(m_arrGameObject[i]);
		}

		m_arrGameObject.clear();
	}
}



