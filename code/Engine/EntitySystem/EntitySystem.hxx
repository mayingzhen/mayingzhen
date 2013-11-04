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
	}

	void EntitySystem::ShoutDown()
	{

	}

	void EntitySystem::Update()
	{

	}

	GameObject*	EntitySystem::CreateGameObject(const char* pName)
	{
		GameObject* pGameObj = new GameObject(pName);	
		m_arrGameObject.push_back(pGameObj);

		return pGameObj;
	}

	void EntitySystem::DeleteGameObject(GameObject* pObject)
	{
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



