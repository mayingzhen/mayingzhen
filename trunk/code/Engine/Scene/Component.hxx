#include "Component.h"

namespace ma
{
	IMPL_OBJECT(Component,Object);

	Component::Component()
	{
		m_pSceneNode = NULL;
	}

	Component::~Component()
	{

	}

	void Component::OnAddToSceneNode(SceneNode* pGameObj)
	{
		ASSERT(pGameObj);
		if (pGameObj == NULL)
			return;

		m_pSceneNode = pGameObj;
	}

	void Component::OnRemoveFromSceneNode(SceneNode* pGameObj)
	{
		ASSERT(m_pSceneNode == pGameObj);
		m_pSceneNode = NULL;
	}

	SceneNode* Component::GetSceneNode()
	{
		return m_pSceneNode;
	}

}

