#include "EntitySystem.h"
#include "Octree.h"

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
		Reset();
	}

	void EntitySystem::ShoutDown()
	{
	}

	void EntitySystem::Update()
	{
		profile_code();
	
		m_pRootGameObject->Update();
	}


	ref_ptr<GameObject>	EntitySystem::CreateGameObject(const char* pName)
	{
		ref_ptr<GameObject>	 pGameObject = new GameObject(pName);	
		m_pRootGameObject->AddChild(pGameObject);
		return pGameObject;
	}

	void EntitySystem::Reset()
	{
		m_pRootGameObject = NULL;
		m_pCullTree = NULL;

		m_pCullTree = new Octree();

		m_pRootGameObject = new GameObject("RootGameObject");		
		m_pRootGameObject->SetCullTree( m_pCullTree.get() );
	}

	void EntitySystem::Serialize(Serializer& sl, const char* pszLable)
	{
		if ( sl.IsReading() )
		{
			Reset();
		}

		sl.BeginSection(pszLable);

		sl.Serialize(*m_pRootGameObject,"RootObject");

		sl.EndSection();
	}

}



