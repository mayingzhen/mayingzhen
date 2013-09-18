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
	}

	void EntitySystem::Start()
	{
		m_pScene->Start();
	}

	void EntitySystem::Stop()
	{
		m_pScene->Stop();
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

		GameObject* pGameObj = new GameObject(pName);	
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



