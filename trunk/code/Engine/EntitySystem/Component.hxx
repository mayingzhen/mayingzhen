#include "Component.h"

namespace ma
{
	IMPL_OBJECT(Component,Object);

	Component::Component(GameObject* pGameObj)
	{
		m_pGameObject = pGameObj;
		m_pSceneNode = pGameObj ? pGameObj->GetSceneNode() : NULL;
	}

	Component::~Component()
	{

	}

// 	void Component::BuildRenderItem()
// 	{
// 
// 	}

	void Component::UpdateTransform()
	{

	}
	
	void Component::Serialize(Serializer& sl, const char* pszLable /*= "Component"*/)
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

