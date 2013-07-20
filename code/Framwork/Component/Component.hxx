#include "Component.h"

namespace ma
{
	IMPL_OBJECT(Component,Object);

	Component::Component(GameObject* pGameObj)
	{
		m_pGameObject = pGameObj;
	}

	Component::~Component()
	{

	}

	void Component::Render()
	{

	}

	void Component::Update()
	{

	}


// 	void Component::ParalleUpdate(float fTimeElapsed)
// 	{
// 
// 	}
// 
// 	void Component::FixedUpdate(float fTimeElapsed)
// 	{
// 
// 	}
// 
// 	void Component::LateUpdate(float fTimeElapsed)
// 	{
// 
// 	}

	void Component::Start()
	{

	}

	void Component::Stop()
	{

	}
	
	GameObject*	Component::GetGameObject()
	{
		return m_pGameObject;
	}

	SceneNode* Component::GetSceneNode() 
	{
		return m_pGameObject ? m_pGameObject->GetSceneNode() : NULL;
	}
}

