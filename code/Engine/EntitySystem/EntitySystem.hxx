#include "EntitySystem.h"
#include "RenderQueueBuilder.h"

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
	
 		m_pCameraObject = CreateGameObject("MainCamera");
 		m_pCamera = m_pCameraObject->CreateComponent<Camera>();
	}

	void EntitySystem::ShoutDown()
	{
		SAFE_DELETE(m_pScene);
	}

	void EntitySystem::Update()
	{
		RenderQueueBuilder rqBuilder(NULL/*m_pCamera*/);
		m_pScene->TravelScene(&rqBuilder);

		rqBuilder.AddToRenderQueue();

		UINT nLight = GetRenderSystem()->GetLightNumber();
		for (UINT i = 0; i < nLight; ++i)
		{
			Light* pLight = GetRenderSystem()->GetLightByIndex(i);
			if ( !pLight->IsCreateShadow() )
				continue;

			pLight->UpdateShadowFrustum(m_pCamera);
			UINT nShadowFrustum = pLight->GetShadowFrustumNumber();
			for (UINT j = 0; j < nShadowFrustum; ++j)
			{
				ShadowMapFrustum* pSMF = pLight->GetShadowFrustumByIndex(j);
				rqBuilder.UpdateCastList(pSMF);
			}
		}
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



